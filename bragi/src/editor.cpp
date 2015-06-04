#include "editor.h"
#include "qmlhighlighter.h"
#include <QDebug>
//#include <QPlainTextDocumentLayout>

Editor::Editor(QQuickItem *parent)
    : QQuickPaintedItem(parent)
    , m_doc(new QTextDocument(this))
    , m_cursor(new QTextCursor(m_doc))
{
//   m_doc->setDocumentLayout(new QPlainTextDocumentLayout(m_doc));
    QMLHighlighter *qmlHigh = new QMLHighlighter(m_doc);
    QFont customFont("Bitstream Vera Sans Mono");
    customFont.setPixelSize(12);
    customFont.setWeight(QFont::Bold);
    m_doc->setDefaultFont(customFont);
    connect(m_doc, SIGNAL(contentsChanged()),
            this, SIGNAL(textChanged()));
    connect(m_doc, SIGNAL(contentsChanged()),
            this, SLOT(update()));
}

QString& Editor::indentify(QString &text)
{
    if (text == QLatin1String("\n") || text == QLatin1String("\r\n") || text == QLatin1String("\r")) {
        QTextBlock b = m_cursor->block();
        int p = m_cursor->positionInBlock();
        if (m_cursor->atBlockStart()) {
            b = b.previous();
            p = b.length() - 1;
        }
        int s = 0;
        QString bl = b.text();
        for(int i=p; i>=0; i--){
            if(bl[i] == '\n')
                break;
            else if(bl[i] == ' ')
                s++;
            else
                s=0;
        }
        QString prevIndent = QString("");
        while(s--)
            prevIndent += " ";
        return (text = QLatin1String("\n")+prevIndent);
    } else {
        return text.replace('\t', "    ");
    }
}

void Editor::paint(QPainter* p)
{
    QPalette pal = QGuiApplication::palette();
    pal.setColor(QPalette::Highlight, m_selectionColor);
    pal.setColor(QPalette::HighlightedText, m_textColor);//TODO: Separate
    pal.setColor(QPalette::Text, m_textColor);
    QAbstractTextDocumentLayout::PaintContext ctx;
    ctx.palette = pal;
    ctx.clip = QRectF(0,0,width(), height());
    m_doc->documentLayout()->draw(p, ctx);
}
