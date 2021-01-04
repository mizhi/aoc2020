import scala.collection._

val INPUT_FILE = "\\\\wsl$\\Ubuntu\\home\\mizhi\\Development\\aoc2020\\day6\\input.txt"

 var bufferedSource = io.Source.fromFile(INPUT_FILE)
 val lines: List[String] = bufferedSource.getLines.toList
 bufferedSource.close()

// This question is just asking to find the set of answers to
// which anyone in the groups of people answered yes. So for each
// group of people, we build a set of the yes answers by finding the
// union of all the responses.
val answerGroups: List[List[Set[Char]]] =
  lines.foldLeft(List.empty[List[Set[Char]]])(
    (accum: List[List[Set[Char]]], line: String) => {
      accum match {
        case _ if line.isBlank => List.empty[Set[Char]] :: accum
        case xs :: xs2 => (line.toSet :: xs) :: xs2
        case Nil => List(line.toSet) :: Nil
      }
    }
  )

val answerSets: List[Set[Char]] =
  answerGroups.map(
    answerGroup => answerGroup.tail.foldLeft(answerGroup.head)(_|_)
  )

println(answerSets.map(_.size).sum)
