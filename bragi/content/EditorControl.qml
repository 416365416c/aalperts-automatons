import QtQuick 2.0

Item {
    property TextEdit targetEditor
    property int targetCursor
    function getValue() {
        if (targetEditor)
            return targetEditor.selectedText
        return 0;
    }

    function setValue(val) {
        if (!targetEditor)
            return;
        var prev = getValue();
        targetEditor.ignoreNextSelection();
        targetEditor.text = targetEditor.text.slice(0, targetCursor) + val
                + targetEditor.text.slice(targetCursor + prev.length);
        targetEditor.select(targetCursor, targetCursor + String(val).length);
    }
}
