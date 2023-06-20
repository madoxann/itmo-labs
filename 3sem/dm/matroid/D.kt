import java.io.File
import java.util.*

fun main() {
    val input = Scanner(File("check.in"))
    val output = File("check.out")

    val (n, m) = input.nextLine().split(" ").map { it.toInt() }
    val independent = mutableSetOf<BitSet>()

    for (i in 0 until m) {
        val members = input.nextLine().split(" ").map { it.toInt() }.drop(1)

        with(BitSet(10)) {
            for (j in members) this.set(j - 1)
            independent += this
        }
    }

    if (!independent.contains(BitSet(10))) {
        output.writeText("NO")
        return
    }

    for (indSet in independent)
        for (i in 0 until n) {
            if (indSet[i] && !independent.contains((indSet.clone() as BitSet).apply { this.clear(i) })) {
                output.writeText("NO")
                return
            }
        }

    for (aSet in independent)
        for (bSet in independent) {
            if (aSet.cardinality() > bSet.cardinality()) {
                val aMinusB = (aSet.clone() as BitSet).apply { this.andNot(bSet) }
                var exists = false

                for (i in 0 until n)
                    if (aMinusB[i])
                        exists = exists || independent.contains((bSet.clone() as BitSet).apply { this.set(i) })

                if (!exists) {
                    output.writeText("NO")
                    return
                }
            }
        }

    output.writeText("YES")
}