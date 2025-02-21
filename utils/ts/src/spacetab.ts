import * as fs from "node:fs"

if (require.main == module) {
  if (process.stdin.isTTY) {
    let [, , a, b] = process.argv
    space2tab(a, parseInt(b))
  } else {
    throw new Error("pipe_not_implemented")
  }
}

function space2tab(file: string, spaces: number): undefined {
  let a = fs
    .readFileSync(file, "utf-8")
    .split("\n")
    .map((a, k) => {
      let m = a.match(/^ +/)
      if (m) {
        let n = m[0].length / spaces
        if (!Number.isInteger(n)) throw new Error(`Not an integer on line ${k}`)

        let i = m.index!
        a = a.slice(0, i) + "\t".repeat(n) + a.slice(i + m[0].length)
      }
      return a
    })
    .join("\n")

  fs.writeFileSync(file, a)
}
