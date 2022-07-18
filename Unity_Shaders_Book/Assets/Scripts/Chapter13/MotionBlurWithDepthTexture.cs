using UnityEngine;

public class MotionBlurWithDepthTexture : PostEffectsBase {
    public Shader motionBlurShader;
    public Material motionBlurMaterial;

    public Material material {
        get {
            motionBlurMaterial = CheckShaderAndCreateMaterial(motionBlurShader, motionBlurMaterial);
            return motionBlurMaterial;
        }
    }

    [Range(0.0f, 1.0f)]
    public float blurSize = 0.5f;

    private Camera myCamera;
    public Camera camera {
        get {
            if (null == myCamera) {
                myCamera = GetComponent<Camera>();
            }
            return myCamera;
        }
    }

    private Matrix4x4 previousViewProjectionMatrix;

    /// <summary>
    /// This function is called when the object becomes enabled and active.
    /// </summary>
    void OnEnable()
    {
        camera.depthTextureMode |= DepthTextureMode.Depth;

        previousViewProjectionMatrix = camera.projectionMatrix * camera.worldToCameraMatrix;
    }

    /// <summary>
    /// OnRenderImage is called after all rendering is complete to render image.
    /// </summary>
    /// <param name="src">The source RenderTexture.</param>
    /// <param name="dest">The destination RenderTexture.</param>
    void OnRenderImage(RenderTexture src, RenderTexture dest)
    {
        if (null != material)
        {
            material.SetFloat("_BlurSize", blurSize);

            material.SetMatrix("_PreviousViewProjectionMatrix", previousViewProjectionMatrix);
            Matrix4x4 currentViewProjectionMatrix = camera.projectionMatrix * camera.worldToCameraMatrix;
            Matrix4x4 currentViewProjectionInverseMatrix = currentViewProjectionMatrix.inverse;
            material.SetMatrix("_CurrentViewProjectionInverseMatrix", currentViewProjectionInverseMatrix);
            previousViewProjectionMatrix = currentViewProjectionMatrix;

            Graphics.Blit(src, dest, material);
        } else {
            Graphics.Blit(src, dest);
        }
    }
}