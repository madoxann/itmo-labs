import java.util.*

data class GenFunction(val c: LongArray) {
    private var negated = false

    operator fun get(index: Int): Long = (if (index !in c.indices) 0 else c[index]) * (if (negated) -1 else 1)

    operator fun unaryMinus() : GenFunction  {
        return apply { negated = !negated }
    }

    fun trim(): GenFunction {
        var cnt = c.size
        val it = c.reversed().iterator()
        while (it.hasNext() && it.next() == 0L) cnt--

        return GenFunction(c.toList().subList(0, cnt).toLongArray())
    }

    fun print() {
        println(c.size - 1)
        print(c.size)
        println()
    }

    fun print(n: Int) {
        for (i in 0 until n) print("${this[i]} ")
    }

    override fun equals(other: Any?): Boolean {
        if (this === other) return true
        if (javaClass != other?.javaClass) return false

        other as GenFunction
        return c.contentEquals(other.c)
    }

    override fun hashCode(): Int {
        return c.contentHashCode()
    }
}


fun build(a: GenFunction, c: GenFunction): GenFunction {
    val sz = a.c.size
    val ans = a.c.clone()

    for (i in 0 until sz) {
        for (j in 1 .. sz)
            ans[i] -= a[i - j] * c[j-1]
    }

    return GenFunction(ans)
}

fun main() {
    val input = Scanner(System.`in`)
    val sz = input.nextInt()

    val a = GenFunction(List(sz) { input.nextLong() }.toLongArray())
    val c = GenFunction(List(sz) { input.nextLong() }.toLongArray())

    build(a, c).trim().print()
    println(sz)
    print("1 ")
    (-c).print(c.c.size)
}
