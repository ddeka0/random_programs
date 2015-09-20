/*

	Debashish Deka 20/09/2015

	input file contain one line
	first integer is = number
	second interger is the choice

example 1:
	infile:

	24 2

	compile and execute:
	javac TestClass2.java
	java TestClass < infile

	console output:
	not prime 


example 2:
	infile:

	-34 1

	compile and execute:
	javac TestClass2.java
	java TestClass < infile

	console output:
	negative 


	no prime checking for negative number is taken care iin theis program

*/



import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.io.OutputStream;
import java.io.PrintWriter;
import java.math.BigInteger;
import java.util.StringTokenizer;


public class TestClass2 {
	public static void main(String[] args) {
		InputStream inputStream = System.in;
		OutputStream outputStream = System.out;
		InputReader in = new InputReader(inputStream);
		PrintWriter out = new PrintWriter(outputStream,true);
		Task task = new Task();	
		int x = in.nextInt();
		int choice = in.nextInt();
		task.check(x,choice,out);
		out.close();
	}
}
abstract class abs_class {
	abstract public void check(int x,int choice,PrintWriter out);
}
class Task extends abs_class {
	private int SIZE = 1000000;
	// prime[x] = true means x is not prime and prime[x] = false means x is prime
	private boolean[] prime = new boolean[SIZE]; // default is false
	// helper function to pre calculate prime numbers
	// for faster access later on
	private void prime_calculation() {
		prime[1] = true;
		for(int i=2;i*i<=SIZE;i++) {
			if(prime[i] == false) {
				for(int j=2;i*j<SIZE;j++) {
					prime[i*j] = true;
				}
			}
		}
	}
	@Override
	public void check(int x, int choice,PrintWriter out) {
		prime_calculation();
		if(choice == 1) {
			if(x > 0) out.println("positive");
			else if( x < 0) out.println("negative");
			else out.println("zero");
		}else if(choice == 2) {
			if(prime[x] == false) out.println("prime");
			else out.println("not prime");
		}else {
			out.println("invalid choice");
		}
	}
}

class InputReader {
    public BufferedReader reader;
    public StringTokenizer tokenizer;
    public InputReader(InputStream stream) {
        reader = new BufferedReader(new InputStreamReader(stream), 32768);
        tokenizer = null;
    }
    public String next() {
        while (tokenizer == null || !tokenizer.hasMoreTokens()) {
            try {
                tokenizer = new StringTokenizer(reader.readLine());
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }
        return tokenizer.nextToken();
    }
    public int nextInt() {
        return Integer.parseInt(next());
    }
    public long nextLong() {
        return Long.parseLong(next());
    }
    public double nextDouble() {
        return Double.parseDouble(next());
    }
    public BigInteger nextbiginteinteger() {
        return new BigInteger(next());
    }
}
