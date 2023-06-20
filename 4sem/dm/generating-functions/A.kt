import java.util.*
import kotlin.math.max


const val MOD = 998244353

data class GenFunction(val c: IntArray) {
    operator fun get(index: Int): Int = if (index !in c.indices) 0 else c[index]

    operator fun plus(other: GenFunction): GenFunction {
        val sz = max(c.size, other.c.size)
        val ans = LongArray(sz)
        for (i in 0 until sz)
            ans[i] = (0L + this[i] + other[i]) % MOD

        return GenFunction(mapToInt(ans))
    }

    operator fun times(other: GenFunction): GenFunction {
        val sz = c.size + other.c.size - 1
        val ans = LongArray(sz)

        for (i in 0 until sz)
            for (j in 0..i)
                ans[i] = (ans[i] + (1L * this[j] * other[i - j])) % MOD

        return GenFunction(mapToInt(ans))
    }

    operator fun div(other: GenFunction): GenFunction {
        val sz = 1000
        val ans = LongArray(sz)

        for (i in 0 until sz) {
            var acc = 0L
            for (j in 1..i) {
                acc = (acc + other[j] * ans[i - j]) % MOD
            }

            ans[i] = ((MOD + this[i] - acc) % MOD) / other[0]
        }

        return GenFunction(mapToInt(ans))
    }

    private fun mapToInt(a: LongArray) = a.toList().map { it.toInt() }.toIntArray()

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

fun main() {
    val input = Scanner(System.`in`)
    val (psz, qsz) = input.nextInt() to input.nextInt()

    val P = GenFunction(List(psz + 1) { input.nextInt() }.toIntArray())
    val Q = GenFunction(List(qsz + 1) { input.nextInt() }.toIntArray())
    var tmp = P + Q
    tmp.print()
    tmp = P * Q
    tmp.print()
    tmp = P / Q
    tmp.print(1000)
}
