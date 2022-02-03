import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
 
public class F {
    private static final Map<Integer, String> vocab = new HashMap<>();
    private static int counterMap = 0;
 
    private static int getLatestCode() {
        while (vocab.containsKey(counterMap)) {
            counterMap++;
        }
        return counterMap;
    }
 
    public static void main(String[] args) {
        String alph = "abcdefghijklmnopqrstuvwxyz";
        for (int i = 0; i < alph.length(); i++) {
            vocab.put(i, Character.toString(alph.charAt(i)));
        }
        Scanner in = new Scanner(System.in);
        int length = in.nextInt();
        int[] encoded = new int[length];
        for (int i = 0; i < length; i++) {
            encoded[i] = in.nextInt();
        }
        in.close();
        String symbol = vocab.get(encoded[0]);
        System.out.print(symbol);
        for (int i = 1; i < length; i++) {
            String str;
            if (vocab.containsKey(encoded[i])) {
                str = vocab.get(encoded[i]);
            } else {
                str = vocab.get(encoded[i-1]);
                str = str + symbol;
            }
            System.out.print(str);
            symbol = Character.toString(str.charAt(0));
            vocab.put(getLatestCode(), vocab.get(encoded[i-1]) + symbol);
        }
    }
}
