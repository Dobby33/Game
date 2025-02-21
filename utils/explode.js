/* See source code at /home/user/Game/utils/ts/src */"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
exports.explode = explode;
if (require.main == module) {
    if (process.stdin.isTTY) {
        throw new Error("tty_not_implemented");
    }
    else {
        throw new Error("pipe_not_implemented");
    }
}
function explode(text) {
    // Assume text is LF
    let a = blank_comments(text);
    let o = [];
    let i = 0;
    let pos = 0;
    // Assume directive has no additional spaces
    i = a.indexOf("#pragma once", i);
    if (i < 0)
        throw new Error("no_pragma");
    i = a.indexOf("\n", i);
    if (i < 0)
        i = a.length;
    o.push({ pos, item: "pragma", head: text.slice(pos, i), body: "" });
    pos = i + 1;
    for (; i < a.length; i++) {
        if (a[i] == "#") {
            let item = "macro";
            let head = text.slice(pos, i); // Comments, etc.
            let body = ""; // Full macro
            for (; i < a.length; i++) {
                body += text[i];
                // Assume escaped sequence indivisible (no comment after \ nor spaces)
                if (a[i] == "\n" && a[i - 1] != "\\")
                    break;
            }
            o.push({ pos, item, head, body });
            pos = i + 1;
        }
        if (a[i] == "{") {
            let item = "scope";
            let head = text.slice(pos, i); // Comments, signature, etc.
            let body = ""; // Function/structure body
            let d = 0;
            for (; i < a.length; i++) {
                body += text[i];
                if (a[i] == "{")
                    d++;
                if (a[i] == "}")
                    d--;
                if (d == 0)
                    break;
            }
            o.push({ pos, item, head, body });
            pos = i + 1;
        }
    }
    o.push({ pos, item: "remains", head: text.slice(pos), body: "" });
    return o;
}
function blank_comments(text, with_symbol = "@") {
    let a = text;
    for (let i = 0; i < a.length; i++) {
        if (a.slice(i, i + 2) == "//") {
            for (; i < a.length; i++) {
                if (a[i] == "\n" && text[i - 1] != "\\") {
                    break;
                }
                a = replace_at(a, i, with_symbol);
            }
        }
        if (a.slice(i, 2) == "\/*") {
            for (; i < a.length; i++) {
                if (a.slice(i, i + 2) == "*\/") {
                    break;
                }
                a = replace_at(a, i, with_symbol);
            }
        }
    }
    return a;
}
function replace_at(dest, i, val) {
    return dest.slice(0, i) + val + dest.slice(i + 1);
}
