import java.util.HashMap;
import java.io.*;
import java.util.NoSuchElementException;
import java.util.Scanner;

public final class H {
    static public HashMap<Integer, Boolean> generatedPriors = new HashMap<>();

    private static class MyReader implements AutoCloseable {
        private final Reader in;

        private final StringBuilder str;
        private int charsRead;
        private int countLines;

        private Integer cachedInt;
        private String cachedLine;
        private String cachedString;

        private boolean lastNewLineRead = false;

        private static final int defaultBufferSize = 1024;

        public MyReader(Reader rd, int sz) {
            this.in = rd;
            this.str = new StringBuilder();
            if (sz <= 0) {
                throw new IllegalArgumentException("Buffer size must be > 0");
            }
            char[] cbuf = new char[sz];
            try {
                int readChars = in.read(cbuf);
                while (readChars != -1) {
                    str.append(cbuf, 0, readChars);
                    readChars = in.read(cbuf);
                }
            } catch (IOException e) {}
            charsRead = 0;
            countLines = 1;
        }

        public MyReader(Reader rd) {
            this(rd, defaultBufferSize);
        }

        public MyReader(InputStream stream) {
            this(new InputStreamReader(stream), defaultBufferSize);
        }

        public MyReader(InputStream stream, int sz) {
            this(new InputStreamReader(stream), sz);
        }

        public MyReader(String str) {
            this(new StringReader(str), defaultBufferSize);
        }

        public MyReader(String str, int sz) {
            this(new StringReader(str), sz);
        }

        public int nextInt() {
            if (cachedInt != null) {
                int tmp = cachedInt;
                cachedInt = null;
                return tmp;
            }
            StringBuilder retInt = new StringBuilder();
            for (int i = charsRead; i < str.length(); i++) {
                char currentChar = str.charAt(i);
                if (Character.isDigit(currentChar)) {
                    retInt.append(currentChar);
                    if (i == str.length() - 1) {
                        charsRead = i + 1;
                        return Integer.parseInt(retInt.toString());
                    }
                } else if (currentChar == '-' && retInt.length() == 0) {
                    retInt.append(currentChar);
                } else {
                    if (retInt.length() > 0) {
                        charsRead = i + 1;
                        return Integer.parseInt(retInt.toString());
                    }
                }
            }
            throw new NoSuchElementException();
        }

        public String nextLine() {
            if (cachedLine != null) {
                String tmp = cachedLine;
                cachedLine = null;
                return tmp;
            }
            for (int i = charsRead; i < str.length(); i++) {
                char currentChar = str.charAt(i);
                if (currentChar == '\n' || currentChar == '\r' || i == str.length() - 1) {
                    int tmp = charsRead;
                    charsRead = i + 1;
                    //Windows fix, looks awful...
                    if (i + 1 < str.length() && System.lineSeparator().equals("" + currentChar + str.charAt(i+1)) ) {
                        charsRead++;
                    }
                    return str.substring(tmp, i);
                }
            }
            return null;
        }

        public String nextWord() {
            if (cachedString != null) {
                String tmp = cachedString;
                cachedString = null;
                return tmp;
            }
            StringBuilder retStr = new StringBuilder();
            for (int i = charsRead; i < str.length(); i++) {
                char currentChar = str.charAt(i);
                lastNewLineRead = false;
                if (Character.isLetter(currentChar) || (Character.getType(currentChar) == Character.DASH_PUNCTUATION) || (currentChar == '\'')) {
                    retStr.append(Character.toLowerCase(currentChar));
                    if (i == str.length() - 1) {
                        charsRead = i + 1;
                        return retStr.toString();
                    }
                } else {
                    if (currentChar == '\n' || System.lineSeparator().equals(String.valueOf(currentChar))) {
                        countLines++;
                        lastNewLineRead = true;
                    }
                    if (retStr.length() > 0) {
                        charsRead = i + 1;
                        return retStr.toString();
                    }
                }
            }
            return null;
        }

        public boolean hasNextInt() {
            if (cachedInt != null) return true;
            try {
                cachedInt = nextInt();
            } catch (NoSuchElementException e) {
                return false;
            }
            return true;
        }

        public boolean hasNextLine() {
            if (cachedLine != null) return true;
            cachedLine = nextLine();
            return cachedLine != null;
        }

        public boolean hasNextWord() {
            if (cachedString != null) return true;
            cachedString = nextWord();
            return cachedString != null;
        }

        public int getCountLines() {
            if (lastNewLineRead) return countLines - 1;
            return countLines;
        }

        public void close() {
            try {
                this.in.close();
            } catch (IOException e) {}
        }
    }

    private static class Node {
        int value;
        int prior;
        Node left;
        Node right;
        int cnt;

        Node(Node l, Node r, int v, int p) {
            left = l;
            right = r;
            value = v;
            prior = p;
            cnt = ((l != null) ? l.cnt : 0) + ((r != null) ? r.cnt : 0) + 1;
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

    private static int getCnt(Node n) {
        return (n == null) ? 0 : n.cnt;
    }
 
    private static int getLCnt(Node n) {
        return (n == null) ? 0 : getCnt(n.left);
    }
 
    private static void recalcCnt(Node n) {
        if (n != null) n.cnt = getCnt(n.left) + getCnt(n.right) + 1;
    }

    private static void split(Node tree, int position, Node[] nodes) {
        if (tree == null) {
            nodes[0] = nodes[1] = null;
            return;
        } 
        int currCnt = getLCnt(tree);
        if (currCnt >= position) {
            Node[] sp = new Node[2];
            split(tree.left, position, sp);
            nodes[0] = sp[0];
            tree.left = sp[1];
            recalcCnt(tree);
            nodes[1] = tree;
            return;
        }
        Node[] sp = new Node[2];
        split(tree.right, position - currCnt - 1, sp);
        tree.right = sp[0];
        recalcCnt(tree);
        nodes[0] = tree;
        nodes[1] = sp[1];
    }

    private static Node merge(Node n1, Node n2) {
        if (n1 == null || n2 == null) return n2 == null ? n1 : n2;
        if (n1.prior > n2.prior) {
            n1.right = merge(n1.right, n2);
            recalcCnt(n1);
            return n1;
        }
        n2.left = merge(n1, n2.left);
        recalcCnt(n2);
        return n2;
    }

   private static void dfsDebug(Node tree)
    {
        if (tree.left != null) {
            dfsDebug(tree.left);
        }
        System.out.print(tree.value + " ");
        if (tree.right != null) {
            dfsDebug(tree.right);
        }
   }

    private static Node insert(Node tree, Node n, int position) {
        Node[] nodes = new Node[2];
        split(tree, position, nodes);
        // Node ret = merge(nodes[0], merge(n, nodes[1]));
        // recalcCnt(ret);
        return merge(merge(nodes[0], n), nodes[1]);
    }

    private static Node delete(Node tree, int position) {
        Node[] nodes = new Node[2];
        split(tree, position, nodes);
        Node[] nodes2 = new Node[2];
        split(nodes[1], 1, nodes2);
        return merge(nodes[0], nodes2[1]);
    }

    private static String out = "";
    private static int nodeCnt = 0;
 
    private static void dfs(Node tree)
    {
        if (tree == null) return;
        dfs(tree.left);
        System.out.print(tree.value + " ");
        nodeCnt++;
        dfs(tree.right);
   }


    public static void main(String[] args) {
        // MyReader in = new MyReader(System.in);
        Scanner in = new Scanner(System.in);
        Node tree = null;
        int n = in.nextInt(), r = in.nextInt();
        nodeCnt = n;
        for (int i = 0; i < n; i++) {
            tree = insert(tree, new Node(in.nextInt()), i);
        }
        for (int i = 0; i < r; i++) {
            String opCode = in.next();
            if (opCode.equals("add")) {
                int pos = in.nextInt(), val = in.nextInt();
                tree = insert(tree, new Node(val), pos);
                nodeCnt++;
            } else {
                tree = delete(tree, in.nextInt()-1);
                nodeCnt--;
            }
        }
        System.out.println(nodeCnt);
        dfs(tree);
        in.close();
    }
}
