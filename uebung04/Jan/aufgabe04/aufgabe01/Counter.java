public class Counter {

    private int count = 0;

    public void increment() {
        int next_free_slot = count;
        next_free_slot += 1;
        count = next_free_slot;
    }

    public int getCount() {
        return count;
    }
}
