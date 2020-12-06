PW_REGEXP = /(?<pos1>\d+)-(?<pos2>\d+) (?<char>.): (?<password>.+)/i

PolicyPassword = Struct.new(:pos1, :pos2, :char, :password)

lines = File.open("input1.txt", "r") { |fi| fi.readlines }

password_policies = lines.map do |line|
  matches = PW_REGEXP.match(line)

  PolicyPassword.new(
    matches[:pos1].to_i - 1,
    matches[:pos2].to_i - 1,
    matches[:char],
    matches[:password]
  )
end

num_valid_pws = password_policies.count do |pp|
  policy_chars = [pp.password[pp.pos1], pp.password[pp.pos2]].uniq
  policy_chars.count == 2 && policy_chars.include?(pp.char)
end

puts num_valid_pws
