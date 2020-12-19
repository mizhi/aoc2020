// Since JavaScript is still a language that I'm not entirely comfortable with,
// I'm making notes as I work through the problem. This is the first problem
// where I've elected a language that I don't feel well-versed in (and an
// environment, Node.js, where I am constantly looking up stuff). This is going
// to result in some potentially wrong or naive notes below.
const fs = require("fs");

// This is used to pick apart the <id>:<value> parts on each line of the
// passport data file. First starts by splitting by whitespace, then
// splits each part by :
let parsePartial = function (line) {
  var parts = line.split(/\s+/);
  return parts.reduce((partial, currentPart) => {
    // could be done with a regex and named capture groups, but that feels
    // like overkill
    var kv = currentPart.split(":");
    partial[kv[0]] = kv[1];
    return partial;
  }, {});
};

// There are values that have common requirements for validation:
// - lower and upper bounds
// - field width
let validateRangeAndWidth = function (x, min_x, max_x, expected_width) {
  let numeric_value = parseInt(x);

  // if x is null or undefined, this is Nan which is always false in numeric
  // comparisons. Therefore, we don't have to do any checks for null or
  // undefiend
  return (
    numeric_value >= min_x &&
    numeric_value <= max_x &&
    x.length === expected_width
  );
};

// FieldValidators is the enhancement from part 1 to simply verifying that all
// the fields are present, as in the first part. This attaches a function to
// each field that does the extra validation steps.
//
// For this problem, we only have properties that correspond to fields that
// we expect to see in the passports.
//
// Structuring in this way allows us to restrict the properties to be the named
// fields and sets us up to use Object.keys(FieldValidators) as we run through
// the passports.
let fieldValidators = {
  // These are all year validations that have lower and upper bounds with
  // a field width of 4. Because we use the validateRangeAndWidth, we
  // don't have to check for the truthiness of x as we do later on.
  byr: (x) => validateRangeAndWidth(x, 1920, 2002, 4),
  iyr: (x) => validateRangeAndWidth(x, 2010, 2020, 4),
  eyr: (x) => validateRangeAndWidth(x, 2020, 2030, 4),

  hgt: (x) => {
    if (!x) return false;

    // use a Regex to pull the string apart. The front part is digits, the
    // second part is a units designation.
    let height = x.match(/^\s*(?<value>\d+)\s*(?<units>in|cm)\s*$/);
    if (!height) return false;

    if (height.groups.units === "cm") {
      return validateRangeAndWidth(height.groups.value, 150, 193, 3);
    } else {
      return validateRangeAndWidth(height.groups.value, 59, 76, 2);
    }
  },

  // this is a pretty straightforward regex to find #rrggbb
  hcl: (x) => {
    if (!x) return false;
    return x.match(/^\s*#[0-9a-f]{6}\s*$/);
  },

  ecl: (x) => ["amb", "blu", "brn", "gry", "grn", "hzl", "oth"].includes(x),
  pid: (x) => {
    if (!x) return false;
    return x.match(/^\s*\d{9}\s*$/);
  },

  cid: (x) => true,
};

let isPassportValid = function (passport) {
  return Object.keys(fieldValidators).every((field) =>
    fieldValidators[field](passport[field])
  );
};

// There's an async version of this that could also work, but this seems cleaner
// for the size of the problem. I used a var because that is not lexically
// scoped in the same way that let or const are. It'd be nice to be able to do
// something like:
//
// let data = try {
//   fs.readFileSync('input.txt', 'utf8').split('\n')
// } catch (err) {
//   console.error(err)
// }
try {
  var data = fs.readFileSync("input.txt", "utf8");
  var lines = data.split("\n");
} catch (err) {
  console.error(err);
}

// The next two reductions could be reduced to a single loop, instead of two, by
// calling isPassportValid as each partial passport is completed and passing
// that as a part of the accumulator. These are separated because the code
// is a bit cleaner (IMO).
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
