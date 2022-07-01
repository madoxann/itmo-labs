import java.util.Scanner;

public class F {
    private static class Node {
        Node left;
        Node right;
        int min;
        Integer query;
        int tl;
        int tr;

        public Node(Node l, Node r) {
            right = r;
            left = l;
            min = Math.min(l.min, r.min);
            this.tl = l.tl;
            this.tr = r.tr;
            query = null;
        }

        public Node(Node l, Node r, int val, int order) {
            right = r;
            left = l;
            min = val;
            this.tl = order;
            this.tr = order + 1;
            query = null;
        }
    }

    public static void push (Node current) {
        if (current.query == null) return;
        current.min = current.query;
        if (current.left == current.right) {
            current.query = null;
            return;
        }
        current.left.query = current.query;
        current.right.query = current.query;
        current.query = null;
    }

    private static Node build(int[] arr, int tl, int tr) {
        if (tl == tr - 1) {
            return new Node(null, null, arr[tl], tl);
        }
        int tm = (tl + tr) / 2;
        Node l = build(arr, tl, tm);
        Node r = build(arr, tm, tr);
        return new Node(l, r);
    }

    private static void update(Node current, int l, int r, int val) {
        if (current.tr <= l || current.tl >= r)
            return;
        if (current.tl >= l && current.tr <= r) {
            current.query = val;
            return;
        }
        push(current);
        update(current.left, l, r, val);
        update(current.right, l, r, val);
        current.min = Math.min(current.left.query != null ? current.left.query : current.left.min,
                current.right.query != null ? current.right.query : current.right.min);
    }

    private static int query(Node current, int l, int r) {
        if (current.tr <= l || current.tl >= r)
            return Integer.MAX_VALUE;
        if (current.tl >= l && current.tr <= r) {
            return current.query != null ? current.query : current.min;
        }
        push(current);
        int ret = Math.min(query(current.left, l, r), query(current.right, l, r));
        current.min = Math.min(current.left.query != null ? current.left.query : current.left.min,
                current.right.query != null ? current.right.query : current.right.min);
        return ret;
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt(), m = in.nextInt();
        int[] arr = new int[n];
        Node tree = build(arr, 0, n);

        for (long i = 0; i < m; i++) {
            long opCode = in.nextInt();
            int l = in.nextInt(), r = in.nextInt();
            if (opCode == 1) {
                int val = in.nextInt();
                update(tree, l, r, val);
            } else
                System.out.println(query(tree, l, r));
        }
        
        in.close();
    }
}