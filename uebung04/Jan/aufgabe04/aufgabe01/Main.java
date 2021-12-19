public class Main {

	private static final int N = 2;

	private static final Thread[] threads = new Thread[N];
	private static final Counter counter = new Counter();

	public static void main(String[] args) throws Exception {
		if (args.length != 1) throw new Exception("USAGE: Main <countMax>");

		int countMax = Integer.parseInt(args[0]);

		for (int i = 0; i < N; i++) {
			threads[i] = new IncrementThread(countMax, counter, i);
			threads[i].start();
		}

		for (int i = 0; i < N; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException ignored) {}
		}

		System.out.printf("EXIT: in=%d%n", counter.getCount());
	}
}