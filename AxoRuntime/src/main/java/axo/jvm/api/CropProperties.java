package axo.jvm.api;

import java.util.ArrayList;
import java.util.List;

public class CropProperties extends BlockProperties {
    public String name = "";
    public String seedItemName = "";
    public String dropItemName = "";
    public List<String> stageTextures = new ArrayList<>();

    public static CropProperties of(){
        return new CropProperties();
    }
    public CropProperties name(String s){
        this.name = s;
        return this;
    }
    public CropProperties seed(String s){
        this.seedItemName = s;
        return this;
    }
    public CropProperties drop(String s){
        this.dropItemName = s;
        return this;
    }
    public CropProperties stageTextures(String s){
        this.stageTextures.add(s);
        return this;
    }
}
