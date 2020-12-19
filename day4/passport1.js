const fs = require("fs");

try {
  var data = fs.readFileSync("input.txt", "utf8");
  var lines = data.split("\n");
} catch (err) {
  console.error(err);
}

function parsePartial(line) {
  var parts = line.split(/\s+/);
  return parts.reduce((partial, currentPart) => {
    var kv = currentPart.split(":");
    partial[kv[0]] = kv[1];
    return partial;
  }, {});
}

const EXPECTED_FIELDS = [
  "byr",
  "iyr",
  "eyr",
  "hgt",
  "hcl",
  "ecl",
  "pid",
  "cid",
];

function isPassportValid(passport) {
  let missing = EXPECTED_FIELDS.filter(
    (x) => !Object.keys(passport).includes(x)
  );

  return missing.length == 0 || (missing.length == 1 && missing[0] == "cid");
}

let passports = lines.reduce((partials, currentLine) => {
  if (currentLine.match(/^\s*$/)) {
    // start a new passport
    partials.push({});
  } else {
    // get the latest passport partial
    partials[partials.length - 1] = {
      ...partials[partials.length - 1],
      ...parsePartial(currentLine),
    };
  }
  return partials;
}, []);

let number_valid = passports.reduce((count, passport) => {
  if (isPassportValid(passport)) {
    count += 1;
  }
  return count;
}, 0);

console.log(number_valid);
