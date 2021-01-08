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

{:ok, body} = File.read("input.txt")

body
|> String.split
|> Enum.map(fn x -> x |> Integer.parse |> elem(0) end)
|> Verifier.find_invalid_code
|> IO.puts
