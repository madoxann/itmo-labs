import java.util.HashMap;
import java.util.Scanner;

public final class Dekartach {
    static int currOrder = 0;

    static public HashMap<Integer, Boolean> generatedPriors = new HashMap<>();

    private static class Node {
        int value;
        int prior;
        Node left;
        Node right;
        int order;

        Node(Node l, Node r, int v, int p) {
            left = l;
            right = r;
            value = v;
            prior = p;
            order = currOrder++;
        }

        Node(int v, int p) {
            this(null, null, v, p);
        }

        Node(int v) {
            this(v, 1);
            if (generatedPriors.containsKey(this.prior)) {
                int newRandom = 2;
                while (generatedPriors.containsKey(newRandom)) {
                    newRandom = (int) ((Math.random() * Integer.MAX_VALUE) % 10000000);    
                }
                this.prior = newRandom;
                generatedPriors.put(this.prior, true);
            } else generatedPriors.put(this.prior, true);
        }
    }

    private static void split(Node tree, int val, Node[] nodes) {
        if (tree == null) {
            nodes[0] = nodes[1] = null;
            return;
        } else if (val <= tree.value) {
            Node[] sp = new Node[2];
            split(tree.left, val, sp);
            tree.left = sp[1];
            nodes[0] = sp[0];
            nodes[1] = tree;
            return;
        }
        Node[] sp = new Node[2];
        split(tree.right, val, sp);
        tree.right = sp[0];
        nodes[0] = tree;
        nodes[1] = sp[1];
    }

    private static Node merge(Node n1, Node n2) {
        if (n1 == null || n2 == null) return n2 == null ? n1 : n2;
        if (n1.prior > n2.prior) {
            n1.right = merge(n1.right, n2);
            return n1;
        }
        n2.left = merge(n1, n2.left);
        return n2;
    }

    private static Node insert(Node tree, Node n) {
        if (tree == null) {
            return n;
        }
        if (tree.prior < n.prior) {
            Node[] nodes = new Node[2];
            split(tree, n.value, nodes);
            n.left = nodes[0];
            n.right = nodes[1];
            return n;
        }
        if (tree.value < n.value) {
            tree.right = insert(tree.right, n);
        } else {
            tree.left = insert(tree.left, n);
        }
        return tree;
    }

    private static void dfs(Node tree)
    {
        System.out.print(tree.order + " ");
        if (tree.left != null) {
            System.out.println(tree.left.order + " " + ((tree.right != null) ? tree.right.order + " " : "0 "));
            dfs(tree.left);
            if (tree.right != null) dfs(tree.right);
        } else if (tree.right != null) {
            System.out.println("0 " + tree.right.order + " ");
            dfs(tree.right);
        } else {
            System.out.println("0 0");
        }
    }

    private static boolean search(Node current, int val) {
        if (current == null) return false;
        if (current.value == val) return true;
        if (val < current.value) return search(current.left, val);
        return search(current.right, val);
    }

    private static String next(Node current, int val) {
        Integer ans = null;
        while (current != null) {
            if (current.value > val) {
                ans = current.value;
                current = current.left;
            }
            else current = current.right;
        }
        return ans != null ? String.valueOf(ans) : "none";
    }

    private static String prev(Node current, int val) {
        Integer ans = null;
        while (current != null) {
            if (current.value < val) {
                ans = current.value;
                current = current.right;
            }
            else current = current.left;
        }
        return ans != null ? String.valueOf(ans) : "none";
    }

    private static Node delete(Node current, int val) {
        if (current.value == val) return merge(current.left, current.right);
        else {
            if (current.value > val) {
                current.left = delete(current.left, val);
            } else {
                current.right = delete(current.right, val);
            }
        }
        return current;
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        Node tree = null;
        while (in.hasNext()) {
            String opCode = in.next();
            int val = in.nextInt();
            switch (opCode) {
                case "insert":
                    if (!search(tree, val)) tree = insert(tree, new Node(val));
                break;
                case "exists":
                    System.out.println(search(tree, val));
                break;
                case "next":
                    System.out.println(next(tree, val));
                break;
                case "prev":
                    System.out.println(prev(tree, val));
                break;
                case "delete":
                    if (search(tree, val)) tree = delete(tree, val);
                break;
                default:
                    System.out.println("No such operation exists");
            }
        }
        in.close();
    }
}
