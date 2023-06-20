import java.io.File
import java.util.*

fun main() {
    val input = Scanner(File("cycles.in"))
    val output = File("cycles.out")

    val (n, m) = input.nextLine().split(" ").map { it.toInt() }
    val groundWeighted = mutableListOf<Pair<Int, Int>>()

    input.nextLine().split(" ").map { it.toInt() }.forEachIndexed { cnt, w ->
        groundWeighted += Pair(w, cnt)
    }
    groundWeighted.sortWith(compareByDescending { it.first })

    val dependant = mutableSetOf<BitSet>()
    for (i in 0 until m)
        with(BitSet(n)) {
            input.nextLine().split(" ").drop(1).forEach {
                this.set(it.toInt() - 1)
            }

            dependant += this
        }

    val maxBase = BitSet(n)
    var ans = 0L
    base@ for (member in groundWeighted) {
        for (cycle in dependant) {
            val united = (maxBase.clone() as BitSet).apply {
                this.set(member.second)
                this.and(cycle)
            }

            if (united == cycle) continue@base
        }

        ans += member.first
        maxBase.set(member.second)
    }

    output.writeText(ans.toString())
}