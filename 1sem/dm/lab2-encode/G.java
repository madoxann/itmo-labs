import java.math.BigDecimal;
import java.math.BigInteger;
import java.math.RoundingMode;
import java.util.Scanner;

public class G {
    static private String word;

    private static class EncodedLetter {
        BigDecimal left;
        BigDecimal right;
        public EncodedLetter(BigDecimal l, BigDecimal r) {
            left = l;
            right = r;
        }
    }

    private static int countChar(char ch) {
        int counter = 0;
        for (int i = 0; i < word.length(); i++) {
            if (word.charAt(i) == ch) {
                counter++;
            }
        }
        return counter;
    }

    public static void main(String[] args) {
        String alph = "abcdefghijklmnopqrstuvwxyz";
        Scanner in = new Scanner(System.in);

        int letCount = in.nextInt();
        word = in.next();
        in.close();

        BigDecimal[] probDiv = new BigDecimal[letCount];
        int sumArr = 0;
        for (int i = 0; i < letCount; i++) {
            probDiv[i] = BigDecimal.valueOf(countChar(alph.charAt(i)));
            sumArr += probDiv[i].intValue();
        }

        System.out.println(letCount);
        for (BigDecimal item : probDiv) {
            System.out.print(item.intValue() + " ");
        }
        System.out.println();

        for (int i = 0; i < letCount; i++) {
            probDiv[i] = probDiv[i].divide(BigDecimal.valueOf(sumArr), 26, RoundingMode.DOWN);
        }

        EncodedLetter[] encodedLetters = new EncodedLetter[letCount];
        for (int i = 0; i < letCount; i++) {
            if (i == 0) {
                encodedLetters[i] = new EncodedLetter(BigDecimal.valueOf(0), probDiv[i]);
            } else if (i == letCount - 1) {
                encodedLetters[i] = new EncodedLetter(encodedLetters[i - 1].right, BigDecimal.valueOf(1));
            } else {
                encodedLetters[i] = new EncodedLetter(encodedLetters[i - 1].right, encodedLetters[i - 1].right.add(probDiv[i]));
            }
        }

        for (int i = 0; i < word.length(); i++) {
            int currentIndex = alph.indexOf(word.charAt(i));
            BigDecimal leftCode = encodedLetters[currentIndex].left;
            BigDecimal rightCode = encodedLetters[currentIndex].right;
            BigDecimal length = rightCode.subtract(leftCode);
            for (int j = 0; j < letCount; j++) {
                if (j == 0) {
                    encodedLetters[j].left = leftCode;
                } else {
                    encodedLetters[j].left = encodedLetters[j - 1].right;
                }
                if (j == letCount - 1) {
                    encodedLetters[j].right = rightCode;
                } else {
                    encodedLetters[j].right = encodedLetters[j].left.add(length.multiply(probDiv[j]));
                }
            }
        }
        BigDecimal left = encodedLetters[0].left;
        BigDecimal right = encodedLetters[encodedLetters.length - 1].right;
        //System.out.println(left + " " + right);

        BigDecimal q = BigDecimal.valueOf(0);
        BigDecimal currQVal = BigDecimal.valueOf(1);
        BigDecimal length = BigDecimal.valueOf(0);
        String code = "";

        infLoop:
        for(;;) {
            BigDecimal l = left.multiply(currQVal);
            BigDecimal r = right.multiply(currQVal);
            for (BigDecimal i = BigDecimal.valueOf(l.intValue()); i.compareTo(BigDecimal.valueOf(r.intValue())) <= 0; i = i.add(BigDecimal.valueOf(1))) {
                BigDecimal currVal = i.divide(currQVal, 7, RoundingMode.DOWN);
                if (left.compareTo(currVal) <= 0 && right.compareTo(currVal) > 0) {
                    code = i.toBigInteger().toString(2);
                    length = q;
                    break infLoop;
                }
            }
            currQVal = currQVal.multiply(BigDecimal.valueOf(2));
            q = q.add(BigDecimal.valueOf(1));
        }

        if (length.subtract(BigDecimal.valueOf(code.length())).compareTo(BigDecimal.valueOf(0)) > 0) {
            System.out.println("0".repeat(length.subtract(BigDecimal.valueOf(code.length())).intValue())+ code);
        } else {
            System.out.println(code);
        }

    }
}
