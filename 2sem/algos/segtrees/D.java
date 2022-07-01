import java.util.Scanner;

public class D {
    private static class Node {
        Node left;
        Node right;
        int amount;

        public Node(Node l, Node r) {
            right = r;
            left = l;
            amount = l.amount + r.amount;
        }

        public Node(Node l, Node r, int val) {
            right = r;
            left = l;
            amount = val;
        }
    }

    private static Node build(int[] arr, int tl, int tr) {
        if (tl == tr) {
            return new Node(null, null, arr[tl]);
        }
        int tm = (tl + tr) / 2;
        Node l = build(arr, tl, tm);
        Node r = build(arr, tm + 1, tr);
        return new Node(l, r);
    }

    private static Node update(Node current, int tl, int tr, int p) {
        if (current.left == current.right) {
            return new Node(null, null, current.amount == 0 ? 1 : 0);
        }
        int tm = (tl + tr) / 2;
        if (p <= tm) {
            current.left = update(current.left, tl, tm, p);
        } else {
            current.right = update(current.right, tm + 1, tr, p);
        }
        return new Node(current.left, current.right);
    }

    private static int query(Node current, int tl, int tr, int p) {
        if (tl == tr) {
            return tl;
        }
        int tm = (tl + tr) / 2;
        if (current.left.amount < p)
            return query(current.right, tm + 1, tr, p - current.left.amount);
        return query(current.left, tl, tm, p);
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        int n = in.nextInt(), m = in.nextInt();
        int[] arr = new int[n];
        for (int i = 0; i < n; i++) arr[i] = in.nextInt();
        Node tree = build(arr, 0, n - 1);

        for (long i = 0; i < m; i++) {
            long opCode = in.nextInt();
            if (opCode == 1) {
                int p = in.nextInt();
                update(tree, 0, n - 1, p);
            } else {
                int p = in.nextInt();
                System.out.println(query(tree, 0, n - 1, p+1));
            }
        }
        
        in.close();
    }
}