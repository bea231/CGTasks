float4x4 u_view_projection_matrix : VIEWPROJECTION;
float4x4 u_model_matrix : MODEL;

float4x4 u_light_plane_matrix;
float4x4 u_light_plane_inv_matrix;

float3   u_camera_pos;

struct vs_out_t
{
  float4 pos : POSITION;
  float4 color : COLOR0;
  float2 tex_coord : TEXCOORD0;
  float3 normal : TEXCOORD1;
  float3 light : TEXCOORD2;
  float3 view : TEXCOORD3;
};

vs_out_t area_light_VS( float3 pos : POSITION, float3 normal : NORMAL, float4 color : COLOR0, float2 tex_coord : TEXCOORD0 )
{
  vs_out_t vs_out = (vs_out_t)0;

  float4 world_pos = mul(float4(pos, 1), u_model_matrix);
  vs_out.pos       = mul(world_pos, u_view_projection_matrix);
  vs_out.color     = color;
  vs_out.tex_coord = tex_coord;
  vs_out.normal    = mul(normal, (float3x3)u_model_matrix);

  float3 light_pos = mul(world_pos, u_light_plane_inv_matrix).xyz;
  light_pos.z = 0;
  light_pos.xy = saturate(light_pos.xy);
  light_pos = mul(float4(light_pos, 1), u_light_plane_matrix).xyz;
  vs_out.light     = light_pos - world_pos.xyz;
  vs_out.view      = u_camera_pos - world_pos.xyz;

  return vs_out;
}


float4 area_light_PS( vs_out_t fs_in ) : COLOR0
{
  float  light_dist = length(fs_in.light);
  float  attenuation = 1.0 / (1.f + 0.005f * light_dist + 0.0001f * light_dist * light_dist);

  float3 light_dir = fs_in.light / max(light_dist, 0.00000001f);
  float3 normal = normalize(fs_in.normal);
  float3 view = normalize(fs_in.view);
  //float  reflective = dot(normalize(view + light_dir), normal); // blinn model
  float  reflective = dot(reflect(-view, normal), light_dir);  // phong model

  float ambient = 0.1;
  float diffuse = saturate(dot(normal, light_dir)) * attenuation + float(light_dist < 0.00000001f);
  float specular = 0.3 * pow(max(0, reflective), 8) * attenuation;
  float lightning = (ambient + diffuse + specular);

  return fs_in.color * lightning;
}

technique area_light_technique
{
  pass P0
  {
    Lighting = false;
    VertexShader = compile vs_2_0 area_light_VS();
    PixelShader  = compile ps_2_0 area_light_PS();
  }
}

