#include <QQuickPaintedItem>
#include <QGuiApplication>
#include <QTextDocument>
#include <QTextBlock>
#include <QAbstractTextDocumentLayout>
#include <QTextCursor>
#include <QClipboard>
#include <QDebug>

class Editor : public QQuickPaintedItem
{
    Q_OBJECT
    Q_PROPERTY(QString text READ text WRITE setText NOTIFY textChanged)
    Q_PROPERTY(int cursorPosition READ cursorPosition WRITE setCursorPosition NOTIFY cursorPositionChanged)
    Q_PROPERTY(QRectF cursorRectangle READ cursorRectangle NOTIFY cursorPositionChanged)
    Q_PROPERTY(int selectionStart READ selectionStart NOTIFY selectionStartChanged)
    Q_PROPERTY(int selectionEnd READ selectionEnd NOTIFY selectionEndChanged)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor NOTIFY textColorChanged)
    Q_PROPERTY(QColor selectionColor READ selectionColor WRITE setSelectionColor NOTIFY selectionColorChanged)
    //### Distant future section
    //font
    //lineCount
public:
    Editor(QQuickItem* parent=0);
    //Needed functions
    QString text() const
    {
        return m_doc->toPlainText();
    }

    int cursorPosition() const
    {
        return m_cursor->position();
    }

    QRectF cursorRectangle() const
    {
        return positionToRectangle(cursorPosition());//### Just So it's bindable, not very efficient
    }

    int selectionStart() const
    {
        return m_cursor->selectionStart();
    }

    int selectionEnd() const
    {
        return m_cursor->selectionEnd();
    }

    QColor textColor() const
    {
        return m_textColor;
    }

    QColor selectionColor() const
    {
        return m_selectionColor;
    }

public slots:
    void update() {
        QQuickPaintedItem::update();
    }

    void setText(QString arg)
    {
        if (m_doc->toPlainText() != arg)
            m_doc->setPlainText(arg);
    }

    void setCursorPosition(int arg)
    {
        if (m_cursorPosition != arg) {
            m_cursor->setPosition(arg);
            emit cursorPositionChanged();//TODO: Anywhere else?
        }
    }

    void setTextColor(QColor arg)
    {
        if (m_textColor != arg) {
            m_textColor = arg;
            emit textColorChanged(arg);
        }
    }

    void setSelectionColor(QColor arg)
    {
        if (m_selectionColor != arg) {
            m_selectionColor = arg;
            emit selectionColorChanged(arg);
        }
    }

    //These are exposed for writing the interaction in QML - operate from cursor
    void insert(QString text)
    {
        m_cursor->insertText(indentify(text));
        emit cursorPositionChanged();
    }

    void del()
    {
        m_cursor->deleteChar();
        emit cursorPositionChanged();
    }

    void delPrev()
    {
        m_cursor->deletePreviousChar();
        emit cursorPositionChanged();
    }

    void redo()
    {
        m_doc->redo(m_cursor);
        emit cursorPositionChanged();
    }

    void undo()
    {
        m_doc->undo(m_cursor);
        emit cursorPositionChanged();
    }

    //Here on down is basically having to re-implement from TextEdit - except we don't have QTextControl :(
    void cut() //Operates on selection
    {
        if (!m_cursor->hasSelection())
            return;
        QGuiApplication::clipboard()->setText(m_cursor->selectedText());
        m_cursor->deleteChar();
        emit cursorPositionChanged();
    }

    void copy() //Operates on selection
    {
        if (!m_cursor->hasSelection())
            return;
        QGuiApplication::clipboard()->setText(m_cursor->selectedText());
        emit cursorPositionChanged();
    }

    void paste() //Operates on selection (kind of)
    {
        if (m_cursor->hasSelection())
            m_cursor->deleteChar();
        m_cursor->insertText(QGuiApplication::clipboard()->text());
        emit cursorPositionChanged();
    }

    void deselect()
    {
        m_cursor->clearSelection();
        emit cursorPositionChanged();
    }

    void select(int start, int end)
    {
        m_cursor->beginEditBlock();
        m_cursor->setPosition(start, QTextCursor::MoveAnchor);
        m_cursor->setPosition(end, QTextCursor::KeepAnchor);
        m_cursor->endEditBlock();
        emit cursorPositionChanged();
    }

    void selectAll()
    {
        m_cursor->select(QTextCursor::Document);
        emit cursorPositionChanged();
    }

    void selectWord() //Closest to cursor
    {
        m_cursor->select(QTextCursor::WordUnderCursor);
        emit cursorPositionChanged();
    }

    int positionAt(int x, int y)
    {
        return m_doc->documentLayout()->hitTest(QPointF(x,y), Qt::FuzzyHit);
    }

    QRectF positionToRectangle(int position) const
    {
        //Literally copied from QQuickTextControlPrivate
        const QTextBlock block = m_doc->findBlock(position);
        if (!block.isValid())
            return QRectF();
        const QAbstractTextDocumentLayout *docLayout = m_doc->documentLayout();
        const QTextLayout *layout = block.layout();
        const QPointF layoutPos = m_doc->documentLayout()->blockBoundingRect(block).topLeft();
        int relativePos = position - block.position();
        QTextLine line = layout->lineForTextPosition(relativePos);

        int cursorWidth;
        bool ok = false;
        cursorWidth = docLayout->property("cursorWidth").toInt(&ok);
        if (!ok)
            cursorWidth = 1;

        QRectF r;

        if (line.isValid()) {
            qreal x = line.cursorToX(relativePos);
            qreal w = 0;
            r = QRectF(layoutPos.x() + x, layoutPos.y() + line.y(), cursorWidth + w, line.height());
        } else {
            r = QRectF(layoutPos.x(), layoutPos.y(), cursorWidth, 10); // #### correct height
        }

        return r;
    }

signals:
    void textChanged();

    void cursorPositionChanged();

    void selectionStartChanged(int arg);

    void selectionEndChanged(int arg);

    void textColorChanged(QColor arg);

    void selectionColorChanged(QColor arg);

protected:
   virtual void paint(QPainter*);
private:
    QString& indentify(QString &text);//assumes text comes after m_cursor (ref for perf?)
    QTextDocument* m_doc;
    QTextCursor* m_cursor;
    int m_cursorPosition;
    int m_selectionStart;
    int m_selectionEnd;
    QColor m_textColor;
    QColor m_selectionColor;
};

