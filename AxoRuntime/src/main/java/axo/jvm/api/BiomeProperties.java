package axo.jvm.api;

public class BiomeProperties {
    public String name = "";
    public int grassColor = 0x7CBD6B;
    public int foliageColor = 0x6BA941;
    public float temperature = 0.5f;
    public float downfall = 0.5f;
    public String topBlock = "grass_block";
    public String fillerBlock = "dirt";

    public static BiomeProperties of(){
        return new BiomeProperties();
    }
    public BiomeProperties name(String s) {
        this.name = s; return this;
    }
    public BiomeProperties grassColor(int c) {
        this.grassColor = c; return this;
    }
    public BiomeProperties foliageColor(int c) {
        this.foliageColor = c; return this;
    }
    public BiomeProperties temperature(float f) {
        this.temperature = f; return this;
    }
    public BiomeProperties downfall(float f) {
        this.downfall = f; return this;
    }
    public BiomeProperties topBlock(String s) {
        this.topBlock = s; return this;
    }
    public BiomeProperties fillerBlock(String s) {
        this.fillerBlock = s; return this;
    }
}
