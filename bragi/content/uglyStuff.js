function findInsertedOpenBrace (newText, oldText) {//Find if the text is the same except for one inserted '{'
    var ret = -1;
    if(newText.length <= oldText.length)
        return -1;
    var pushed = false;
    for(var i =0; i<newText.length; i++) {
        if (!pushed) {
            if (oldText.charAt(i) != newText.charAt(i)){
                if (newText.charAt(i) == '{') {
                    ret = i;
                    pushed = true;
                } else {
                    return -1;
                }
            }
        } else {
            if (oldText.charAt(i-1) != newText.charAt(i))
                return -1;
        }
    }
    return ret;
}
