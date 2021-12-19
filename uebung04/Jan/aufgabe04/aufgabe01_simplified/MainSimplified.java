public class MainSimplified {

	private static final int N = 2;

	private static final Thread[] threads = new Thread[N];
	private static int in = 0;

	public static void main(String[] args) throws Exception {
		if (args.length != 1) throw new Exception("USAGE: MainSimplified <countMax>");

		int countMax = Integer.parseInt(args[0]);

		for (int i = 0; i < N; i++) {
			threads[i] = new Thread(() -> {
				for (int j = 0; j < countMax; j++) {
					int next_free_slot = in;
					next_free_slot += 1;
					in = next_free_slot;
				}
			});

			threads[i].start();
		}

		for (int i = 0; i < N; i++) {
			try {
				threads[i].join();
			} catch (InterruptedException ignored) {}
		}

		System.out.printf("EXIT: in=%d%n", in);
	}
}