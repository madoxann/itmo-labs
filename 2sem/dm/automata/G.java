import java.io.*;
import java.nio.charset.Charset;
import java.util.LinkedList;
import java.util.NoSuchElementException;
import java.util.Queue;

public final class G {
    private static class FastReader implements AutoCloseable {
        private final Reader in;

        private final StringBuilder str;
        private int charsRead;
        private int countLines;

        private Integer cachedInt;
        private String cachedLine;
        private String cachedString;

        private boolean lastNewLineRead = false;

        private static final int defaultBufferSize = 1024;

        public FastReader(Reader rd, int sz) {
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
            } catch (IOException e) {
            }
            charsRead = 0;
            countLines = 1;
        }

        public FastReader(Reader rd) {
            this(rd, defaultBufferSize);
        }

        public FastReader(InputStream stream) {
            this(new InputStreamReader(stream), defaultBufferSize);
        }

        public FastReader(InputStream stream, int sz) {
            this(new InputStreamReader(stream), sz);
        }

        public FastReader(String str) {
            this(new StringReader(str), defaultBufferSize);
        }

        public FastReader(String str, int sz) {
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
                    // Windows fix, looks awful...
                    if (i + 1 < str.length() && System.lineSeparator().equals("" + currentChar + str.charAt(i + 1))) {
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
                if (Character.isLetter(currentChar) || (Character.getType(currentChar) == Character.DASH_PUNCTUATION)
                        || (currentChar == '\'')) {
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
            if (cachedInt != null)
                return true;
            try {
                cachedInt = nextInt();
            } catch (NoSuchElementException e) {
                return false;
            }
            return true;
        }

        public boolean hasNextLine() {
            if (cachedLine != null)
                return true;
            cachedLine = nextLine();
            return cachedLine != null;
        }

        public boolean hasNextWord() {
            if (cachedString != null)
                return true;
            cachedString = nextWord();
            return cachedString != null;
        }

        public int getCountLines() {
            if (lastNewLineRead)
                return countLines - 1;
            return countLines;
        }

        public void close() {
            try {
                this.in.close();
            } catch (IOException e) {
            }
        }
    }

    private static class Pair {
        public int first;
        public int second;

        Pair(int a, int b) {
            first = a;
            second = b;
        }
    }

    private static class Automata {
        public int[][] transitions;
        public boolean[] terminal;

        private final int stateN;

        public Automata(FastReader in ,int s, int tr, int term) {
            stateN = s;
            terminal = new boolean[stateN+1];
            for (int i = 0; i < term; i++)
                terminal[in.nextInt()] = true;
            transitions = new int[stateN+1][26];
            for (int i = 0; i < tr; i++)
            {
                int start = in.nextInt(), end = in.nextInt();
                transitions[start][in.nextWord().charAt(0) - 'a'] = end;
            }
        }

        public boolean isTerminal(int state) { return terminal[state]; }
    }

    private static boolean bfs (Automata a1, Automata a2) {
        LinkedList<Pair> states = new LinkedList<>();
        boolean[][] visited = new boolean[a1.stateN + 1][a2.stateN + 1];
        states.add(new Pair(1, 1));
        while (!states.isEmpty()) {
            Pair current = states.getFirst();
            states.remove(0);
            int s1 = current.first;
            int s2 = current.second;
            visited[s1][s2] = true;
            if (a1.isTerminal(s1) != a2.isTerminal(s2)) return false;
            for (int i = 0; i < 26; i++) {
                int newS1 = a1.transitions[s1][i];
                int newS2 = a2.transitions[s2][i];
                if (!visited[newS1][newS2]) states.add(new Pair(newS1, newS2));
            }
        }
        return true;
    }

    public static void main(String[] args) {
        FastReader in = null;
        try {
            in = new FastReader(new FileInputStream("equivalence.in"));
        } catch (FileNotFoundException e) {
            // well, fie should be there
        }
        assert in != null;
        Automata a1 = new Automata(in, in.nextInt(), in.nextInt(), in.nextInt());
        Automata a2 = new Automata(in, in.nextInt(), in.nextInt(), in.nextInt());
        in.close();

        boolean ans = bfs(a1, a2);
        try {
            BufferedWriter out = new BufferedWriter(new FileWriter("equivalence.out"));
            out.write(ans ? "YES" : "NO");
            out.flush();
            out.close();
        } catch (IOException e) {
            // error handling not necessary
        }
    }
}