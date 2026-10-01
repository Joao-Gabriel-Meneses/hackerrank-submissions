import Page;

public class BTreeMap<Key extends Comparable<Key>, Value> {
    private Page<Key, Value> root = new Page<>(true);

    public BTreeMap(Key sentinel) { 
        put(sentinel, null); 
    }

    public Value get(Key key) { 
        return get(root, key);
    }

    private Value get(Page<Key, Value> h, Key key) {
        if (h.isExternal()) return h.getValue(key);
        return get(h.next(key), key);
    }

    public void put(Key key, Value val) {
        put(root, key, val);
        if (root.hasOverflowed()) {
            Page<Key, Value> lefthalf = root;
            Page<Key, Value> righthalf = root.split();
            root = new Page<>(false);
            root.enter(lefthalf);
            root.enter(righthalf);
        }
    }

    private void put(Page<Key, Value> h, Key key, Value val) {
        if (h.isExternal()) { 
            h.insert(key, val); 
            return; 
        }
        Page<Key, Value> next = h.next(key);
        put(next, key, val);
        if (next.hasOverflowed()) {
            // Insere o primeiro elemento da metade direita na página pai
            h.enter(next.split()); 
        }
        next.close();
    }
}