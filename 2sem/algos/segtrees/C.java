import java.io.*;
import java.util.NoSuchElementException;

public final class C {
    public static class MyReader implements AutoCloseable {
        private Reader in;

        private char[] cbuf;
        private int charsRead, charsNext;

        private String chachedInt;
        private String chachedLine;

        interface Compareble {
            boolean comparator(char in);
        }

        private Compareble cmp;

        private boolean EOF = false;

        private static int defaultBufferSize = 8192;
        private static int defaultExpectedLineLength = 80;

        public MyReader(Reader rd, int sz) {
            this.in = rd;
            if (sz <= 0) {
                throw new IllegalArgumentException("Buffer size must be > 0");
            }
            cbuf = new char[sz];
            charsNext = charsRead = 0;
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

        private void fill() {
            int n = 0;
            try {
                n = in.read(cbuf);
            } catch (IOException e) {
                // LMAO
            }
            if (n > 0) {
                charsRead = n;
                charsNext = 0;
            }
        }

        private String read() {
            StringBuilder s = null;
            int startChar;
            for (; ; ) {
                if (charsNext == charsRead) {
                    fill();
                }
                //EOF
                if (charsNext == charsRead) {
                    EOF = true;
                    if (s != null && (s.length() > 0)) {
                        return s.toString();
                    } else {
                        return null;
                    }
                }

                boolean eol = false;
                int i;

                for (i = charsNext; i < charsRead; i++) {
                    char c = cbuf[i];
                    if (cmp.comparator(c)) {
                        eol = true;
                        break;
                    }
                }

                startChar = charsNext;
                charsNext = i;

                if (eol) {
                    String str;
                    if (s == null) {
                        str = new String(cbuf, startChar, charsNext - startChar);
                    } else {
                        s.append(cbuf, startChar, charsNext - startChar);
                        str = s.toString();
                    }
                    charsNext++;
                    return str;
                }

                if (s == null) {
                    s = new StringBuilder(defaultExpectedLineLength);
                }
                s.append(cbuf, startChar, i - startChar);
            }
        }

        int nextInt() {
            String possibleInt;
            if (chachedInt != null) {
                possibleInt = chachedInt;
                chachedInt = null;
                return Integer.parseInt(possibleInt);
            }
            cmp = in -> {
                if (Character.isWhitespace(in)) {
                    return true;
                }
                return false;
            };
            while (!EOF) {
                possibleInt = read();
                try {
                    int foundInt = Integer.parseInt(possibleInt);
                    //getting cache
                    while (!EOF) {
                        chachedInt = read();
                        try {
                            int foundCache = Integer.parseInt(chachedInt);
                            chachedInt = String.valueOf(foundCache);
                            break;
                        } catch (NumberFormatException e) {
                            continue;
                        }
                    }
                    //end of getting cache
                    return foundInt;
                } catch (NumberFormatException e) {
                    continue;
                }
            }
            throw new NoSuchElementException();
        }

        public String nextLine() throws IOException {

            String foundLine;
            if (chachedLine != null) {
                foundLine = chachedLine;
                chachedLine = null;
                return foundLine;
            }
            cmp = in -> {
                if (in == '\n' || in == '\r') {
                    return true;
                }
                return false;
            };
            foundLine = read();
            if (foundLine != null) {
                chachedLine = read();
            }
            return foundLine;

        }

        public boolean hasNextInt() throws IOException {
            if (chachedInt != null) {
                return true;
            }
            if (!EOF) {
                //getting cache
                cmp = in -> {
                    if (Character.isWhitespace(in)) {
                        return true;
                    }
                    return false;
                };
                while (!EOF) {
                    chachedInt = read();
                    try {
                        int foundCache = Integer.parseInt(chachedInt);
                        chachedInt = String.valueOf(foundCache);
                        return true;
                    } catch (NumberFormatException e) {
                        continue;
                    }
                }
                //got the cache
            }
            return false;
        }

        public boolean hasNextLine() throws IOException {
            if (chachedLine != null) {
                return true;
            }
            if (!EOF) {
                //getting cache
                cmp = in -> {
                    if (in == '\n' || in == '\r') {
                        return true;
                    }
                    return false;
                };
                chachedLine = read();
                if (chachedLine != null) {
                    return true;
                }
                //got the cache
            }
            return false;
        }

        public void close() throws IOException {
            this.in.close();
        }
    }

    private static class Node {
        Node left;
        Node right;
        long ans;
        long whole;
        long suff;
        long pref;

        public Node(Node l, Node r) {
            right = r;
            left = l;
            this.whole = l.whole + r.whole;
            this.pref = Math.max(l.pref, l.whole + r.pref);
            this.suff = Math.max(r.suff, r.whole + l.suff);
            this.ans = Math.max(l.suff + r.pref, Math.max(l.ans, r.ans));
        }

        public Node(Node l, Node r, long val) {
            right = r;
            left = l;
            this.whole = val;
            this.pref = Math.max(0, val);
            this.suff = this.pref;
            this.ans = this.pref;
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

    private static Node update(Node current, int tl, int tr, int p, long val) {
        if (current.left == current.right) {
            return new Node(current.left, current.left, val);
        }
        int tm = (tl + tr) / 2;
        if (p <= tm) {
            current.left = update(current.left, tl, tm, p, val);
        } else {
            current.right = update(current.right, tm + 1, tr, p, val);
        }
        return new Node(current.left, current.right);
    }

    private static Node query(Node current, long tl, long tr, long l, long r) {
        if (l > r) return new Node(null, null, 0);
        if (l == tl && r == tr) {
            return current;
        }
        long tm = (tl + tr) / 2;
        return new Node(query(current.left, tl, tm, l, Math.min(r, tm)), query(current.right, tm + 1, tr, Math.max(l, tm + 1), r));
    }

    public static void main(String[] args) {
        MyReader in = new MyReader(System.in);
        int n = in.nextInt(), m = in.nextInt();
        int[] arr = new int[n];
        for (int i = 0; i < n; i++) arr[i] = in.nextInt();
        Node tree = build(arr, 0, n - 1);
        System.out.println(query(tree, 0, n - 1, 0, n - 1).ans);
        for (int i = 0; i < m; i++) {
            int ind = in.nextInt();
            int val = in.nextInt();
            tree = update(tree, 0, n - 1, ind, val);
            System.out.print(query(tree, 0, n - 1, 0, n - 1).ans + "\n");
        }
        
        in.close();
    }
}
