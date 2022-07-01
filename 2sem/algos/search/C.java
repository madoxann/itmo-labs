import java.util.Scanner;

public final class C {
    private static class Node {
        long value;
        Node left;
        Node right;

        public Node(long v, Node l, Node r) {
            value = v;
            left = l;
            right = r;
        }

        public Node(long v) {
            this(v, null, null);
        }
    }

    private static void insert(Node root, long inserted) {
        while (root != null) {
            if (root.value <= inserted) {
                if (root.right != null) root = root.right;
                else {
                    root.right = new Node(inserted);
                    return;
                }
            } else {
                if (root.left != null) root = root.left;
                else {
                    root.left = new Node(inserted);
                    return;
                }
            }
        }
    }

    private static Node delete(Node current, long val) {
        if (current == null) return null;
        if (current.value > val) current.left = delete(current.left, val);
        else if (current.value < val) current.right = delete(current.right, val);
        else if (current.left != null && current.right != null) {
            Node traverse = current.right;
            while (traverse.left != null) traverse = traverse.left;
            current.value = traverse.value;
            current.right = delete(current.right, current.value);
        }
        else current = (current.right == current.left) ? null : (current.right == null) ? current.left : current.right;
        return current;
    }

    private static boolean search(Node current, long val) {
        if (current == null) return false;
        if (current.value == val) return true;
        if (val < current.value) return search(current.left, val);
        return search(current.right, val);
    }

    private static String next(Node current, long val) {
        Long ans = null;
        while (current != null) {
            if (current.value > val) {
                ans = current.value;
                current = current.left;
            }
            else current = current.right;
        }
        return ans != null ? String.valueOf(ans) : "none";
    }

    private static String prev(Node current, long val) {
        Long ans = null;
        while (current != null) {
            if (current.value < val) {
                ans = current.value;
                current = current.right;
            }
            else current = current.left;
        }
        return ans != null ? String.valueOf(ans) : "none";
    }

    public static void main(String[] args) {
        Scanner in = new Scanner(System.in);
        Node tree = null;
        while (in.hasNext()) {
            String opCode = in.next();
            int val = in.nextInt();
            switch (opCode) {
                case "insert":
                    if (tree == null) {
                        tree = new Node(val);
                        break;
                    }
                    if (!search(tree, val)) insert(tree, val);
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
                    System.out.println("ABOBA");
            }
        }
        in.close();
    }
}