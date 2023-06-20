import java.io.File
import java.util.*

class DSU(n: Int) {
    private var data: IntArray

    init {
        data = IntArray(n)

        for (i in 0 until n) data[i] = i
    }

    fun find(elem: Int): Int = if (elem == data[elem]) elem else find(data[elem]).also { data[elem] = it }

    private fun unite(a: Int, b: Int) {
        data[find(b)] = data[find(a)]
    }

    fun findMST(edges: MutableList<Edge>, ans: MutableSet<Int>, indBegin: Int): MutableSet<Int> {
        val colored = mutableSetOf<Int>()

        edges.forEachIndexed { cnt, edge ->
            if (ans.contains(cnt) && cnt != indBegin) {
                colored += edge.color

                if (find(edges[cnt].begin) != find(edges[cnt].end))
                    unite(edges[cnt].begin, edges[cnt].end)
            }
        }

        return colored
    }
}

data class Edge(
    val begin: Int,
    val end: Int,
    val color: Int
)

fun getPathMap(n: Int, m: Int, data: MutableList<Edge>, ans: MutableSet<Int>) : Map<Int, MutableList<Int>> {
    val currMap = mutableMapOf<Int, MutableList<Int>>()

    for (i in 0 until m) {
        if (ans.contains(i)) {
            val dsu = DSU(n)
            val colors = dsu.findMST(data, ans, i)

            data.forEachIndexed { cnt, edge ->
                if (!ans.contains(cnt)) {
                    if (dsu.find(edge.begin) != dsu.find(edge.end)) currMap.getOrPut(i) { mutableListOf() } += cnt
                    if (!colors.contains(edge.color)) currMap.getOrPut(cnt) { mutableListOf() } += i
                }
            }
        }
    }

    return currMap
}

fun getPath(ind: Int, mapPath: Map<Int, MutableList<Int>>, path: List<Array<Int>>, w: List<Array<Int>>) {
    val acc = mutableListOf<Int>()

    mapPath[ind]?.forEach {
        if (w[0][ind] < w[0][it] - 1) {
            path[0][it] = ind
            w[0][it] = w[0][ind] + 1
            acc += it
        }
    }

    acc.forEach {
        getPath(it, mapPath, path, w)
    }
}

fun main() {
    val input = Scanner(File("rainbow.in"))
    val output = File("rainbow.out")

    val (n, m) = input.nextLine().split(" ").map { it.toInt() }

    val listOfEdges = mutableListOf<Edge>()
    for (i in 0 until m) {
        val (b, e, c) = input.nextLine().split(" ").map { it.toInt() }

        listOfEdges += Edge(b - 1, e - 1, c)
    }

    val ans = mutableSetOf<Int>()
    while (true) {
        var dp = Int.MAX_VALUE
        val toBeColored = mutableListOf<Int>()
        val findRec = LinkedList<Int>()
        val w = Array(m) { dp }

        val dsu = DSU(n)
        val colors = dsu.findMST(listOfEdges, ans, -1)
        listOfEdges.forEachIndexed { cnt, edge ->
            if (!ans.contains(cnt)) {
                if (!colors.contains(edge.color)) toBeColored += cnt

                if (dsu.find(edge.begin) != dsu.find(edge.end)) {
                    w[cnt] = 0
                    findRec.add(cnt)
                }
            }
        }

        val path = Array(m) { -1 }
        val pathMap = getPathMap(n, m, listOfEdges, ans)
        findRec.forEach {
            getPath(it, pathMap, listOf(path), listOf(w))
        }

        var ind = -1
        for (edge in toBeColored) {
            if (w[edge] < dp) ind = edge
            if (dp > w[edge]) dp = w[edge]
        }

        if (ind < 0) break
        while (ind >= 0) {
            if (ans.contains(ind)) ans.remove(ind)
            else ans += ind
            ind = path[ind]
        }
    }

    output.printWriter().use {
        it.println(ans.size)
        for (edge in ans) it.print("${edge + 1} ")
    }
}