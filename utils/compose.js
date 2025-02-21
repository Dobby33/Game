/* See source code at /home/user/Game/utils/ts/src */"use strict";
Object.defineProperty(exports, "__esModule", { value: true });
const path = require("node:path");
const fs = require("node:fs");
const explode_1 = require("./explode");
if (require.main == module) {
    if (process.stdin.isTTY) {
        let [, , a, b, c] = process.argv;
        compose(a, b, c);
    }
    else {
        throw new Error("pipe_not_implemented");
    }
}
function compose(src, head = "head.h", types = "types.h") {
    let headers = fs
        .readdirSync(src)
        .filter((e) => e.endsWith(".h"))
        .filter((e) => e != types)
        .filter((e) => e != head);
    let fragments = headers.map((file) => ({
        file,
        list: (0, explode_1.explode)(fs.readFileSync(path.join(src, file), "utf-8")),
    }));
    let signatures = fragments.map(({ file, list }) => ({
        file,
        list: list.filter((e) => e.item == "scope").map((e) => e.head + ";"),
    }));
    let headers_from_files = fragments.map(({ file, list }) => ({
        file,
        list: list
            .filter((e) => e.item == "macro")
            .filter((e) => e.body.startsWith("#include"))
            .map((e) => e.body),
    }));
    fs.writeFileSync(path.join(src, head), [
        `// THIS FILE WAS CREATED AUTOMATICALLY BY ${process.argv[1]}`,
        ``,
        `#pragma once`,
        ``,
        ...headers_from_files
            .filter((e) => e.list.length)
            .map((e) => [
            `// HEADERS FROM ${e.file}`,
            ``,
            e.list.join(""), // Macros are already have new line character at the end
        ].join("\n")),
        ``,
        `// USER TYPES HEADER`,
        ``,
        `#include "${types}"`,
        ...signatures.map((e) => [``, `// SIGNATURES FROM ${e.file}`, ...e.list].join("\n")),
        ``,
        `// USER HEADERS`,
        ``,
        ...headers.map((e) => `#include "${e}"`),
    ].join("\n"));
}
