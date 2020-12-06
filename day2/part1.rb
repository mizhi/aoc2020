PW_REGEXP = /(?<min_occur>\d+)-(?<max_occur>\d+) (?<char>.): (?<password>.+)/i

PolicyPassword = Struct.new(:min_occur, :max_occur, :char, :password)

lines = File.open("input1.txt", "r") { |fi| fi.readlines }

password_policies = lines.map do |line|
  matches = PW_REGEXP.match(line)

  PolicyPassword.new(
    matches[:min_occur].to_i,
    matches[:max_occur].to_i,
    matches[:char],
    matches[:password]
  )
end

num_valid_pws = password_policies.count do |pp|
  num_chars = pp.password.count(pp.char)
  (pp.min_occur..pp.max_occur).include?(num_chars)
end

puts num_valid_pws
