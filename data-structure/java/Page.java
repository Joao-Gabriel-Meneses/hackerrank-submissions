import java.util.AbstractMap;
import java.util.ArrayList;
import java.util.Collections;
import java.util.List;

public class Page<Key extends Comparable<Key>, Value> {
    private boolean isExternal;
    // Lista para guardar os pares (Chave, Valor) ordenados dentro da página
    private final List<Map.Entry<Key, Value>> entries = new ArrayList<>();
    // Lista de ponteiros para as sub-páginas filhas (usada se não for externa)
    private final List<Page<Key, Value>> children = new ArrayList<>();

    // Ordem da árvore B (número máximo de filhos/elementos por página)
    private static final int M = 4; 

    public Page(boolean isExternal) {
        this.isExternal = isExternal;
    }

    public boolean isExternal() {
        return isExternal;
    }

    // Verifica se a página estourou a capacidade máxima
    public boolean hasOverflowed() {
        return entries.size() >= M;
    }

    // Busca se a chave está presente em uma página externa (folha)
    public boolean holds(Key key) {
        return getIndex(key) >= 0;
    }

    // Retorna o valor associado à chave, se existir
    public Value getValue(Key key) {
        int idx = getIndex(key);
        if (idx >= 0) {
            return entries.get(idx).getValue();
        }
        return null;
    }

    // Retorna a sub-página correta para continuar a busca/inserção
    public Page<Key, Value> next(Key key) {
        for (int i = 0; i < entries.size(); i++) {
            if (key.compareTo(entries.get(i).getKey()) < 0) {
                return children.get(i);
            }
        }
        return children.get(entries.size());
    }

    // Insere ou atualiza um par (chave, valor) na página externa
    public void insert(Key key, Value val) {
        int idx = getIndex(key);
        if (idx >= 0) {
            // Se a chave já existe, atualiza o valor (comportamento de Map)
            entries.set(idx, new AbstractMap.SimpleEntry<>(key, val));
        } else {
            // Se não existe, insere e mantém a lista ordenada
            entries.add(new AbstractMap.SimpleEntry<>(key, val));
            entries.sort(Map.Entry.comparingByKey());
        }
    }

    // Adiciona um filho (usado no split e na raiz)
    public void enter(Page<Key, Value> child) {
        children.add(child);
    }

    // Divide a página ao meio quando ocorre overflow
    public Page<Key, Value> split() {
        int mid = entries.size() / 2;
        Page<Key, Value> rightHalf = new Page<>(this.isExternal);

        // Move a metade superior para a nova página (direita)
        rightHalf.entries.addAll(entries.subList(mid, entries.size()));
        entries.subList(mid, entries.size()).clear();

        // Se não for externa, divide os ponteiros dos filhos também
        if (!isExternal) {
            rightHalf.children.addAll(children.subList(mid + 1, children.size()));
            children.subList(mid + 1, children.size()).clear();
        }

        return rightHalf;
    }

    // Método auxiliar para encontrar a posição de uma chave na lista
    private int getIndex(Key key) {
        for (int i = 0; i < entries.size(); i++) {
            if (key.compareTo(entries.get(i).getKey()) == 0) {
                return i;
            }
        }
        return -1;
    }
}