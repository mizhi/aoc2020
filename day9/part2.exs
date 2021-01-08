defmodule Verifier do
  # Generates all possible pairs [x, y] of numbers in codes where x != y.
  def generate_pairs(codes) do
    for x <- codes, y <- codes, x != y, do: [x, y]
  end

  # Takes a chunk of numbers and verifies that there is at least one pair in the
  # 1..nth numbers that sum to the 0th number.
  # Verifies that code == x + y are two different numbers from previous_codes.
  def find_valid_pairs([code|previous_codes]) do
    previous_codes
    |> Verifier.generate_pairs
    |> Enum.filter(fn [x,y] -> code == x + y end)
  end

  # Returns true if the codes in chunk are valid.
  def valid_code_chunk?(chunk) do
    chunk
    |> Enum.reverse
    |> Verifier.find_valid_pairs
    |> Enum.count != 0
  end

  # Identifies the first invalid code in codes when chunked into 26 number
  # chunks.
  def find_invalid_code(codes, chunk_size \\ 26) do
    codes
    |> Enum.chunk_every(chunk_size, 1)
    |> Enum.find([], fn chunk -> !Verifier.valid_code_chunk?(chunk) end)
    |> List.last
  end
end

# This is not the most efficient implementation because it builds all possible
# sublists of the codes and then searches through the sublists to find the one
# that sums to the invalid code.
#
# A more efficient implementation would lazily build lists and stop at the
# first valid sublist.
defmodule Cracker do
  # Builds all sublists of codes that are at least 2 numbers in length.
  # So, if the list is [1,2,3,4]
  # then this will build the lists:
  # [
  #  [1,2], [1,2,3], [1,2,3,4],
  #  [2,3], [2,3,4],
  #  [3,4]
  # ]
  def make_sublists(invalid_code, codes) do
    for x_start <- 0..(length(codes) - 2),
      x_amount <- 2..(length(codes) - x_start) do
        Enum.slice(codes, x_start, x_amount)
    end
  end

  def find_weakness(invalid_code, codes) do
    codes
    |> Cracker.make_sublists
    |> Enum.find(fn sub_list -> Enum.sum(sub_list) == invalid_code end)
    |> Enum.min_max
    |> Tuple.to_list
    |> Enum.sum
  end
end

{:ok, body} = File.read("input.txt")


# This could, potentially be more elegant by returning both the invlid code and
# the complete sublist from element 0 to the invalid element in the list from
# Verifier.find_invalid_code in a tuple. This would allow chaining (|>) from
# Verifier.find_inavlid_code into Cracker.find_weakness without saving the
# intermediate. The way I have it structured now, I have to save into the codes
# variable to pass into Cracker.find_weakness.
codes = body
|> String.split
|> Enum.map(fn x -> x |> Integer.parse |> elem(0) end)

codes
|> Verifier.find_invalid_code
|> Cracker.find_weakness(codes)
|> IO.puts
