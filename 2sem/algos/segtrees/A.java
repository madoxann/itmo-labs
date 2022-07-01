import java.util.Scanner;

public final class A {
    private static class Node {
        Node left;
        Node right;
        long value;

        public Node(Node l, Node r, long val) {
            right = r;
            left = l;
            value = val;
        }
    }

    private static Node build(long[] arr, int tl, int tr) {
        if (tl == tr) {
            return new Node(null, null, arr[tl]);
        }
        int tm = (tl + tr) / 2;
        Node l = build(arr, tl, tm);
        Node r = build(arr, tm + 1, tr);
        return new Node(l, r, l.value + r.value);
    }

    private static void update(Node current, int tl, int tr, int p, long val) {
        if (current.left == current.right) {
            current.value = val;
            return;
        }
        int tm = (tl + tr) / 2;
        if (p <= tm) {
            update(current.left, tl, tm, p, val);
        } else {
            update(current.right, tm + 1, tr, p, val);
        }
        current.value = current.left.value + current.right.value;
    }

    private static long query(Node current, long tl, long tr, long l, long r) {
        if (l > r) return 0;
        if (l == tl && r == tr) {
            return current.value;
        }
        long tm = (tl + tr) / 2;
        return query(current.left, tl, tm, l, Math.min(r, tm)) + query(current.right, tm + 1, tr, Math.max(l, tm+1), r);
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt(), m = in.nextInt();
        long[] arr = new long[n];
        for (int i = 0; i < n; i++) arr[i] = in.nextLong();
        Node tree = build(arr, 0, n-1);

        for (long i = 0; i < m; i++) {
            long opCode = in.nextInt();
            if (opCode == 1) {
                int p = in.nextInt();
                long val = in.nextLong();
                update(tree, 0, n-1, p, val);
            } else {
                long l = in.nextInt(), r = in.nextInt();
                System.out.println(query(tree, 0, n-1, l, r-1));
            }
        }

        in.close();
    }
}
