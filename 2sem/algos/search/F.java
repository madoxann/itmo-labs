import java.util.HashMap;
import java.util.Scanner;

public final class F {
    static int currOrder = 0;

    static public HashMap<Long, Boolean> generatedPriors = new HashMap<>();

    private static class Node {
        long value, sum;
        long prior;
        Node left;
        Node right;

        Node(Node l, Node r, long v, long p) {
            left = l;
            right = r;
            value = v;
            prior = p;
            sum = v;
        }

        Node(long v, long p) {
            this(null, null, v, p);
        }

        Node(long v) {
            this(v, 1);
            if (generatedPriors.containsKey(this.prior)) {
                long newRandom = 2;
                while (generatedPriors.containsKey(newRandom)) {
                    newRandom = (int) ((Math.random() * Integer.MAX_VALUE) % 10000000);    
                }
                this.prior = newRandom;
                generatedPriors.put(this.prior, true);
            } else generatedPriors.put(this.prior, true);
        }

        void recalc() {
            sum = value + ((left != null) ? left.sum : 0) + ((right != null) ? right.sum : 0); 
        }
    }

    private static void split(Node tree, long val, Node[] nodes) {
        if (tree == null) {
            nodes[0] = nodes[1] = null;
            return;
        } else if (val <= tree.value) {
            Node[] sp = new Node[2];
            split(tree.left, val, sp);
            tree.left = sp[1];
            nodes[0] = sp[0];
            nodes[1] = tree;
            if (nodes[0] != null) nodes[0].recalc();
            if (nodes[1] != null) nodes[1].recalc();
            return;
        }
        Node[] sp = new Node[2];
        split(tree.right, val, sp);
        tree.right = sp[0];
        nodes[0] = tree;
        nodes[1] = sp[1];
        if (nodes[0] != null) nodes[0].recalc();
        if (nodes[1] != null) nodes[1].recalc();
    }

    private static Node merge(Node n1, Node n2) {
        if (n1 == null || n2 == null) return n2 == null ? n1 : n2;
        if (n1.prior > n2.prior) {
            n1.right = merge(n1.right, n2);
            n1.recalc();
            return n1;
        }
        n2.left = merge(n1, n2.left);
        n2.recalc();
        return n2;
    }

    private static boolean search(Node current, long val) {
        if (current == null) return false;
        if (current.value == val) return true;
        if (val < current.value) return search(current.left, val);
        return search(current.right, val);
    }

    private static Node insertSingle(Node tree, Node n) {
        if (search(tree, n.value)) return tree;
        Node[] nodes = new Node[2];
        split(tree, n.value, nodes);
        Node[] nodes2 = new Node[2];
        split(nodes[1], n.value + 1, nodes2);
        if (nodes2[0] == null) nodes2[0] = n;
        Node ret = merge(nodes[0], merge(nodes2[0], nodes2[1]));
        ret.recalc();
        return ret;
    }

    private static long sumBetween(Node[] tree, long a, long b) {
        Node[] nodes = new Node[2];
        Node treeCopy = tree[0];
        split(treeCopy, a, nodes);
        Node[] nodes2 = new Node[2];
        split(nodes[1], b + 1, nodes2);
        long ans = nodes2[0] != null ? nodes2[0].sum : 0; 
        tree[0] = merge(nodes[0], merge(nodes2[0], nodes2[1]));
        if (tree[0] != null) tree[0].recalc();
        return ans;
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        Node tree = null;
        long n = in.nextInt();
        Long prevQVal = null;
        for (long i = 0; i < n; i++) {
            String opCode = in.next();
            if (opCode.equals("+")) {
                if (prevQVal != null) {
                    long val = ((in.nextInt() + prevQVal) % 1000000000);
                    tree = insertSingle(tree, new Node(val));
                    prevQVal = null;
                } else {
                    tree = insertSingle(tree, new Node(in.nextInt()));
                } 
            } else {
                prevQVal = sumBetween(new Node[] {tree}, in.nextInt(), in.nextInt());
                System.out.println(prevQVal);    
            }
        }
 
        in.close();
    }
}