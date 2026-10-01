//chave sentinela menor que qualquer chave que um cliente venha a inserir

public class BTreeSet<Key extends Comparable<Key>> {

   private Page root = new Page(true);

   public BTreeSet(Key sentinel) { 
      inserir(sentinel); 
   }

   public boolean busca(Key key) { 
      return busca(root, key); 
   }

   private boolean busca(Page h, Key key) {
      if (h.isExternal()) return h.holds(key);
      return busca(h.next(key), key);
   }

   public void inserir(Key key) {
      inserir(root, key);
      if (root.hasOverflowed()) {
         Page lefthalf = root;
         Page righthalf = root.split();
         root = new Page(false);
         root.enter(lefthalf);
         root.enter(righthalf);
      }
   }

   public void inserir(Page h, Key key) {
      if (h.isExternal()) { 
         h.insert(key); 
         return; 
      }
      Page next = h.next(key);
      inserir(next, key);
      if (next.hasOverflowed())
         h.enter(next.split());
      next.close();
   }
}
