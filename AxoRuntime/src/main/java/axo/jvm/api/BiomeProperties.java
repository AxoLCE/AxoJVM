package axo.jvm.api;

public class BiomeProperties {
    public enum Category{
        DESERT(0), WARM(1), COOL(2), ICY(3);
        public final int id;
        Category(int id){
            this.id = id;
        }
    }
    public String name = "";
    public int grassColor = 0x7CBD6B;
    public int foliageColor = 0x6BA941;
    public float temperature = 0.5f;
    public float downfall = 0.5f;
    public float depth = 0.1f;
    public float scale = 0.4f;
    public String topBlock = "grass_block";
    public String fillerBlock = "dirt";
    public Category spawnCategory = null;
    public int spawnWeight = 0;
    public int waterColor = 0x3F76E4;
    public int skyColor = 0x78A7FF;
    public Dimension spawnDimension = null;
    public int dimensionSpawnWeight = 0;

    public static BiomeProperties of(){
        return new BiomeProperties();
    }
    public BiomeProperties name(String s) {
        this.name = s; return this;
    }
    public BiomeProperties grassColor(int c) {
        this.grassColor = c;
        return this;
    }
    public BiomeProperties foliageColor(int c) {
        this.foliageColor = c;
        return this;
    }
    public BiomeProperties temperature(float f) {
        this.temperature = f;
        return this;
    }
    public BiomeProperties downfall(float f) {
        this.downfall = f;
        return this;
    }
    public BiomeProperties depth(float f){
        this.depth = f;
        return this;
    }
    public BiomeProperties scale(float f){
        this.scale = f;
        return this;
    }
    public BiomeProperties topBlock(String s) {
        this.topBlock = s;
        return this;
    }
    public BiomeProperties fillerBlock(String s) {
        this.fillerBlock = s;
        return this;
    }
    public BiomeProperties spawnIn(Category category, int weight){
        this.spawnCategory = category;
        this.spawnWeight = weight;
        return this;
    }

    public BiomeProperties spawnInDimension(Dimension dimension, int weight){
        this.spawnDimension = dimension;
        this.dimensionSpawnWeight = weight;
        return this;
    }

    public BiomeProperties waterColor(int c) {
        this.waterColor = c; return this;
    }
    public BiomeProperties skyColor(int c) {
        this.skyColor = c; return this;
    }
}
