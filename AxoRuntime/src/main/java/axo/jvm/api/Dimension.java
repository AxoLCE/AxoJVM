package axo.jvm.api;

public enum Dimension {
    OVERWORLD(0),
    NETHER(1),
    END(2);
    public final int id;
    Dimension(int id){
        this.id = id;
    }
}
