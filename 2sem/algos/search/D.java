import java.util.Scanner;
     
public final class D {
    private static class Node {
        long value;
        long height;
        Node left;
        Node right;
 
        public Node(long v, Node l, Node r) {
            value = v;
            left = l;
            right = r;
            height = 0;
        }
 
        public Node(long v) {
            this(v, null, null);
        }
    }
 
    private static Node rotateLeft(Node n) {
        if (n == null) return n;
        Node l = n.right;
        Node tmp = l.left;
        l.left = n;
        n.right = tmp;
        n.height = recalc(n);
        l.height = recalc(l);
        return l;
    }
 
    private static Node rotateRight(Node n) {
        if (n == null) return n;
        Node l = n.left;
        Node tmp = l.right;
        l.right = n;
        n.left = tmp;
        n.height = recalc(n);
        l.height = recalc(l);
        return l;
    }
 
    private static long recalc(Node n) {
        if (n == null) return 0;
        return Math.max(getHeight(n.left), getHeight(n.right)) + 1;
    }
 
    private static long diff(Node n) { return (n != null) ? (getHeight(n.left) - getHeight(n.right)) : 0; }
 
    private static Node insert(Node root, long inserted) {
        if (root == null) return new Node(inserted);
        if (inserted < root.value) root.left = insert(root.left, inserted);
        else if (inserted > root.value) root.right = insert(root.right, inserted);
        else return root;
        return balanceInsert(root, inserted);
    }
 
    private static long getHeight(Node n) {
        if (n == null) return 0;
        return n.height;
    }
 
    private static Node balanceInsert(Node root, long inserted) {
        if (root == null) return null;
        root.height = recalc(root);
        long diff = diff(root);
        if (Math.abs(diff) <= 1) return root;
        if (diff > 1) {
            if (root.left == null) return null;
            if (inserted < root.left.value) return rotateRight(root);
            root.left = rotateLeft(root.left);
            return rotateRight(root);
        }
        if (root.right == null) return null;
        if (inserted > root.right.value) return rotateLeft(root);
        root.right = rotateRight(root.right);
        return rotateLeft(root); 
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
 
        if (current == null) return null;
        current.height = recalc(current);
        long diff = diff(current);
        if (Math.abs(diff) <= 1) return current;
        if (diff > 1) {
            if (diff(current.left) >= 0) return rotateRight(current);
            current.left = rotateLeft(current.left);
            return rotateRight(current);
        }
        if (diff(current.right) <= 0) return rotateLeft(current);
        current.right = rotateRight(current.right);
        return rotateLeft(current);
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
            } else current = current.right;
        }
        return ans != null ? String.valueOf(ans) : "none";
    }
 
    private static String prev(Node current, long val) {
        Long ans = null;
        while (current != null) {
            if (current.value < val) {
                ans = current.value;
                current = current.right;
            } else current = current.left;
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
                    if (!search(tree, val)) tree = insert(tree, val);
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