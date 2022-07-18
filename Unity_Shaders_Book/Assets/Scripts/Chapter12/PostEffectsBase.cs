using UnityEngine;

[ExecuteInEditMode]
[RequireComponent(typeof(Camera))]
public class PostEffectsBase : MonoBehaviour {
    protected void CheckResources() {
        if (!CheckSupport()) {
            enabled = false;
        }
    }

    protected void Start() {
        CheckResources();
    }

    protected bool CheckSupport() {
        if (SystemInfo.supportsImageEffects == false || SystemInfo.supportsRenderTextures == false) {
            Debug.LogWarning("This platform does not support image effects or render textures.");
            return false;
        }

        return true;
    }

    protected Material CheckShaderAndCreateMaterial(Shader shader, Material material) {
        if (null == shader) return null;

        if (shader.isSupported && material && material.shader == shader)
            return material;

        if (!shader.isSupported)
        {
            return null;
        } else {
            material = new Material(shader);
            material.hideFlags = HideFlags.DontSave;
            if (material)
                return material;
            else
                return null;
        }
    }
}