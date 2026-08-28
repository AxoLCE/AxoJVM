package axo.jvm.api;

import axo.jvm.ItemRegistry;

import java.util.ArrayList;
import java.util.List;

public class CropBlock extends Block{
    private final CropProperties cropProps;
    public CropBlock(CropProperties props) {
        super(BlockProperties.of().name(props.name));
        this.cropProps = props;
    }

    public String[] getStageTexture(){
        List<String> prefixed = new ArrayList<>();
        for (String tex : cropProps.stageTextures){
            prefixed.add("axo:" + getModId() + ":" + tex);
        }
        return prefixed.toArray(new String[0]);
    }
    public int getSeedItemId(){
        return ItemRegistry.getItemIdByName(cropProps.seedItemName);
    }
    public int getDropItemId(){
        return ItemRegistry.getItemIdByName(cropProps.dropItemName);
    }
}
