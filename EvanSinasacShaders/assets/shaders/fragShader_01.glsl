// Fragment shader
#version 420

in vec4 fVertexColour;			// The vertex colour from the original model
in vec4 fVertWorldLocation;
in vec4 fNormal;
in vec4 fUVx2;
in vec4 fDebugColourOverride;	// If debug normals are being drawn, this is the colour

// Replaces gl_FragColor
// Feeney changed this so it's not used anymore, trying to figure out what he does use instead
//out vec4 pixelColour;			// RGB A   (0 to 1) 

// The "whole object" colour (diffuse and specular)
uniform vec4 wholeObjectDiffuseColour;	// Whole object diffuse colour
uniform bool bUseWholeObjectDiffuseColour;	// If true, the whole object colour is used (instead of vertex colour)
uniform vec4 wholeObjectSpecularColour;	// Colour of the specular highlight (optional)

// Alpha Transparency value
uniform float wholeObjectAlphaTransparency;

// This is used for wireframe or whole object colour. 
// If bUseDebugColour is TRUE, then the fragment colour is "objectDebugColour".
uniform bool bUseDebugColour;	
uniform vec4 objectDebugColour;		
// If this is true, then the vertex is drawn with no lighting and this colour	
uniform bool bDrawDebugNormals;			// Default is 0 (or FALSE)

// This will not modulate the colour by the lighting contribution.
// i.e. shows object colour "as is". 
// Used for wireframe or debug type objects
uniform bool bDontLightObject;			// 1 if you want to AVOID lighting

// for the imposter models
uniform bool bIsImposter;

// This is the camera eye location (update every frame)
uniform vec4 eyeLocation;

// added
uniform vec2 screenWidthHeight;

struct sLight
{
	vec4 position;			
	vec4 diffuse;	
	vec4 specular;	// rgb = highlight colour, w = power
	vec4 atten;		// x = constant, y = linear, z = quadratic, w = DistanceCutOff
	vec4 direction;	// Spot, directional lights
	vec4 param1;	// x = lightType, y = inner angle, z = outer angle, w = TBD
	                // 0 = pointlight
					// 1 = spot light
					// 2 = directional light
	vec4 param2;	// x = 0 for off, 1 for on
};

const int POINT_LIGHT_TYPE = 0;
const int SPOT_LIGHT_TYPE = 1;
const int DIRECTIONAL_LIGHT_TYPE = 2;


const int NUMBEROFLIGHTS = 100;
uniform sLight theLights[NUMBEROFLIGHTS];  	// 80 uniforms
// 
// uniform vec4 theLights[0].position;
// uniform vec4 theLights[0].diffuse;
// ...
// uniform vec4 theLights[1].position;
// uniform vec4 theLights[1].diffuse;
// ...

vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular );

uniform sampler2D texture_00;		//GL_TEXTURE_2D
uniform sampler2D texture_01;		//GL_TEXTURE_2D
uniform sampler2D texture_02;		//GL_TEXTURE_2D
uniform sampler2D texture_03;		//GL_TEXTURE_2D

uniform sampler2D texture_04;		//GL_TEXTURE_2D
uniform sampler2D texture_05;		//GL_TEXTURE_2D
uniform sampler2D texture_06;		//GL_TEXTURE_2D
uniform sampler2D texture_07;		//GL_TEXTURE_2D

// For deferred rendering
// Indicates which 'pass' we are doing
const uint PASS_1_G_BUFFER_PASS = 1;	// Renders only geometry to G-Buffer
const uint PASS_2_LIGHT_PASS = 2;		// Apply lighting to G-Buffer
const uint PASS_3_2D_EFFECTS_PASS = 3;		// Optional effects (blur, whatever...)
uniform uint renderPassNumber;

// On the 2nd pass (light pass), I'll read from these ones
// So I'll connect the FBO vertexMaterialColour_1_ID texture to texVertexMaterialColour.
uniform sampler2D texVertexMaterialColour;
uniform sampler2D texVertexNormal;
uniform sampler2D texVertexWorldPos;
uniform sampler2D texVertexSpecular;

uniform sampler2D texScope;
uniform sampler2D texWindow;

// making new ones for the smaller screens
uniform sampler2D screenTex1VertexMaterialColour;
uniform sampler2D screenTex1VertexNormal;
uniform sampler2D screenTex1VertexWorldPos;
uniform sampler2D screenTex1VertexSpecular;

uniform sampler2D screenTex2VertexMaterialColour;
uniform sampler2D screenTex2VertexNormal;
uniform sampler2D screenTex2VertexWorldPos;
uniform sampler2D screenTex2VertexSpecular;

uniform sampler2D screenTex3VertexMaterialColour;
uniform sampler2D screenTex3VertexNormal;
uniform sampler2D screenTex3VertexWorldPos;
uniform sampler2D screenTex3VertexSpecular;

uniform sampler2D screenTex4VertexMaterialColour;
uniform sampler2D screenTex4VertexNormal;
uniform sampler2D screenTex4VertexWorldPos;
uniform sampler2D screenTex4VertexSpecular;


// On the 3rd pass (effects pass), we'll read from this ones
uniform sampler2D texLightPassColourBuffer;


uniform sampler2D tex2VertexMaterialColour;
uniform sampler2D tex2VertexNormal;
uniform sampler2D tex2VertexWorldPos;
uniform sampler2D tex2VertexSpecular;

uniform sampler2D tex2LightPassColourBuffer;	// don't seem to actually be using this so I'll use it for bloom
uniform bool bloom;
uniform bool useBloom;
uniform bool horizontal;
uniform float weight[5] = float[] ( 0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162 );
uniform float exposure;

uniform bool calcBloomBrightness;
uniform sampler2D texBloomBrightness;

uniform vec4 texture2DRatios0to3;
uniform vec4 texture2DRatios4to7;

// Cube maps for skybox, etc.
uniform samplerCube cubeMap_00;
uniform samplerCube cubeMap_01;
uniform samplerCube cubeMap_02;
uniform samplerCube cubeMap_03;
uniform vec4 cubeMap_Ratios0to3;

// If true, then we only sample from the cube maps
uniform bool bIsSkyBox;

// If this is true, then we will sample the "discardTexture" to
// perform a discard on that pixel
// (we could also do a change in the transparency, or something)
uniform sampler2D discardTexture;
uniform bool bDiscardTransparencyWindowsOn;

// texture operator instead of always adding
// 0 - Addition
// 1 - Subtraction
// 2 - Multiplication
// 3 - Division
// 4 - Normal mapping
uniform int textureOperator;

uniform vec2 flickerOffset;
uniform vec2 cam1StaticOffset;
uniform vec2 cam2StaticOffset;

uniform int twoDEffectOperator;
const float offset = 1.0 / 300.0;

// This is used for a discard example 
// Everything inside the "black hole" isn't drawn
uniform float blackHoleSize;

uniform bool bUseAllLights;

// Skybox or reflection or light probe
//uniform samplerCube skyBox;			// GL_TEXTURE_CUBE_MAP

// I think this is what replaces pixelColour
// added
// Replaces gl_FragColor
out vec4 pixelOutputFragColour;			// RGB Alpha   (0 to 1) 
// These make up the G-Buffer:
out vec4 pixelOutputMaterialColour;		// = 1;		rga (w unused)
out vec4 pixelOutputNormal;				// = 2;		xyz (w unused)
out vec4 pixelOutputWorldPos;			// = 3;		xyz w = 0 if lit, 1 if unlit
out vec4 pixelOutputSpecular;			// = 4;		rgb, w = power
const float G_BUFFER_OBJECT_NOT_LIT = 0.0f;
const float G_BUFFER_LIT = 1.0f;

in vec3 fTangentViewPos;
in vec3 fTangentFragPos;
in vec3 fTangentLightPos;
in mat3 fTBN;

uniform vec4 stencilColour;

void main()
{
	// This is the pixel colour on the screen.
	// Just ONE pixel, though.

	pixelOutputFragColour.rgba = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	//pixelOutputFragColour.a = wholeObjectAlphaTransparency;

	// bloom
		if (bloom && renderPassNumber != PASS_3_2D_EFFECTS_PASS)
		{
			vec2 UVlookup;
			UVlookup.x = gl_FragCoord.x / screenWidthHeight.x;	// Width
			UVlookup.y = gl_FragCoord.y / screenWidthHeight.y;	// Height
			//vec2 tex_offsets = 1.0 / textureSize (texLightPassColourBuffer, 0);
			vec3 result = pixelOutputFragColour.rgb * weight[0];
//			if (horizontal)
//			{
//				for (int i = 1; i < 5; i++)
//				{
//					result += texture ( texBloomBrightness, vec2( UVlookup.x + offset * i, UVlookup.y )).rgb * weight[i];
//					//result += texture ( texVertexMaterialColour, vec2( UVlookup.x + offset * i, UVlookup.y )).rgb * weight[i];
//					//result += texture ( texLightPassColourBuffer, UVlookup + vec2(tex_offsets.x * i, 0.0)).rgb * weight[i];
//					result += texture ( texBloomBrightness, vec2( UVlookup.x - offset * i, UVlookup.y )).rgb * weight[i];
//					//result += texture ( texVertexMaterialColour, vec2( UVlookup.x - offset * i, UVlookup.y )).rgb * weight[i];
//					//result += texture ( texLightPassColourBuffer, UVlookup - vec2(tex_offsets.x * i, 0.0)).rgb * weight[i];
//				}
//			}
//			else
//			{
//				for (int i = 1; i < 5; i++)
//				{
//					result += texture ( texBloomBrightness, vec2( UVlookup.x, UVlookup.y + offset * i)).rgb * weight[i];
//					//result += texture ( texVertexMaterialColour, vec2( UVlookup.x, UVlookup.y + offset * i)).rgb * weight[i];
//					//result += texture ( texLightPassColourBuffer, UVlookup + vec2(tex_offsets.y * i, 0.0)).rgb * weight[i];
//					result += texture ( texBloomBrightness, vec2( UVlookup.x, UVlookup.y - offset * i)).rgb * weight[i];
//					//result += texture ( texVertexMaterialColour, vec2( UVlookup.x, UVlookup.y - offset * i)).rgb * weight[i];
//					//result += texture ( texLightPassColourBuffer, UVlookup - vec2(tex_offsets.y * i, 0.0)).rgb * weight[i];
//				}
//			}

			for (int i = 1; i < 5; i++)
			{
				result += texture( texBloomBrightness, vec2( UVlookup.x + offset / 2.0 * i, UVlookup.y )).rgb * weight[i];
				result += texture( texBloomBrightness, vec2( UVlookup.x - offset / 2.0 * i, UVlookup.y )).rgb * weight[i];
				result += texture( texBloomBrightness, vec2( UVlookup.x, UVlookup.y + offset / 2.0 * i)).rgb * weight[i];
				result += texture( texBloomBrightness, vec2( UVlookup.x, UVlookup.y - offset / 2.0 * i)).rgb * weight[i];
			}

			pixelOutputFragColour = vec4(result, 1.0);


			pixelOutputNormal = vec4(normalize(fNormal.xyz), 1.0f);
			pixelOutputMaterialColour = vec4(pixelOutputFragColour.rgb, 1.0f);
			pixelOutputWorldPos = vec4(fVertWorldLocation.xyz, 1.0f);
			pixelOutputWorldPos.w = G_BUFFER_LIT;
			pixelOutputSpecular = wholeObjectSpecularColour.rgba;			// = 4;	
			return;
		}

	if ( calcBloomBrightness )
	{	// https://learnopengl.com/Advanced-Lighting/Bloom
		vec2 UVlookup;
		UVlookup.x = gl_FragCoord.x / screenWidthHeight.x;	// Width
		UVlookup.y = gl_FragCoord.y / screenWidthHeight.y;	// Height
		pixelOutputFragColour.rgb = texture( texLightPassColourBuffer, UVlookup ).rgb;
		pixelOutputFragColour.a = 1.0f;
		float brightness = dot( pixelOutputFragColour.rgb, vec3( 0.2126, 0.7152, 0.0722 ));
		//float brightness = ( pixelOutputFragColour.r + pixelOutputFragColour.g + pixelOutputFragColour.b ) / 3.0;
		if ( brightness < 0.95 )
		{
			pixelOutputFragColour = vec4( 0.0, 0.0, 0.0, 1.0);
		}

		pixelOutputNormal = vec4(normalize(fNormal.xyz), 1.0f);
		pixelOutputMaterialColour = vec4(pixelOutputFragColour.rgb, 1.0f);
		pixelOutputWorldPos = vec4(fVertWorldLocation.xyz, 1.0f);
		pixelOutputWorldPos.w = G_BUFFER_LIT;
		pixelOutputSpecular = wholeObjectSpecularColour.rgba;			// = 4;	
		return;
	}
	
//    _____     ____   ____    _____   __   __              _          ____                  
//   |___ / _  |___ \ |  _ \  | ____| / _| / _|  ___   ___ | |_  ___  |  _ \  __ _  ___  ___ 
//     |_ \(_)   __) || | | | |  _|  | |_ | |_  / _ \ / __|| __|/ __| | |_) |/ _` |/ __|/ __|
//    ___) |_   / __/ | |_| | | |___ |  _||  _||  __/| (__ | |_ \__ \ |  __/| (_| |\__ \\__ \
//   |____/(_) |_____||____/  |_____||_|  |_|   \___| \___| \__||___/ |_|    \__,_||___/|___/
//                                                                                           
	// Optional effects (blur, whatever...)
	if ( renderPassNumber == PASS_3_2D_EFFECTS_PASS )
	{
		// Render the texture to the quad, and that's it
		vec2 UVlookup;
		UVlookup.x = gl_FragCoord.x / screenWidthHeight.x;	// Width
		UVlookup.y = gl_FragCoord.y / screenWidthHeight.y;	// Height
		vec3 sampleColour;

		vec2 offsets[9] = vec2[](
					vec2(-offset,  offset), // top-left
					vec2( 0.0f,    offset), // top-center
					vec2( offset,  offset), // top-right
					vec2(-offset,  0.0f),   // center-left
					vec2( 0.0f,    0.0f),   // center-center
					vec2( offset,  0.0f),   // center-right
					vec2(-offset, -offset), // bottom-left
					vec2( 0.0f,   -offset), // bottom-center
					vec2( offset, -offset)  // bottom-right    
		);

		// This is where you could do full screen effects like chromatic aberration, blur, etc. 
		// https://learnopengl.com/Advanced-OpenGL/Framebuffers
		if (twoDEffectOperator == 1)
		{	// Inversion
			sampleColour = vec3(1.0 - texture( texLightPassColourBuffer, UVlookup )).rgb;
		}
		else if (twoDEffectOperator == 2)
		{	// Greyscale
			sampleColour = texture ( texLightPassColourBuffer, UVlookup ).rgb;
			float average = (sampleColour.r + sampleColour.g + sampleColour.b) / 3.0;
			sampleColour = vec3(average, average, average);
		}
		else if (twoDEffectOperator == 3)
		{	// Sharpen Kernel
			float kernel[9] = float[](
					-1, -1, -1,
					-1,  9, -1,
					-1, -1, -1
			);
			
			vec3 sampleTex[9];
			for (int i = 0; i < 9; i++)
			{
				sampleTex[i] = vec3(texture(texLightPassColourBuffer, UVlookup + offsets[i]));
			}
			vec3 col = vec3(0.0);
			for (int i = 0; i < 9; i++)
				col += sampleTex[i] * kernel[i];

			sampleColour = col;
		}
		else if (twoDEffectOperator == 4)
		{	// Blur Kernal

			float kernel[9] = float[](
					1.0 / 16, 2.0 / 16, 1.0 / 16,
					2.0 / 16,  4.0 / 16, 2.0 / 16,
					1.0 / 16, 2.0 / 16, 1.0 / 16
			);
			vec3 sampleTex[9];
			for (int i = 0; i < 9; i++)
			{
				sampleTex[i] = vec3(texture(texLightPassColourBuffer, UVlookup + offsets[i]));
			}
			vec3 col = vec3(0.0);
			for (int i = 0; i < 9; i++)
				col += sampleTex[i] * kernel[i];

			sampleColour = col;
		}
		else if (twoDEffectOperator == 5)
		{	// Edge sharpen
			float kernel[9] = float[](
					1.0, 1.0, 1.0,
					1.0, -8.0, 1.0,
					1.0, 1.0, 1.0
			);
			vec3 sampleTex[9];
			for (int i = 0; i < 9; i++)
			{
				sampleTex[i] = vec3(texture(texLightPassColourBuffer, UVlookup + offsets[i]));
			}
			vec3 col = vec3(0.0);
			for (int i = 0; i < 9; i++)
				col += sampleTex[i] * kernel[i];

			sampleColour = col;
		}
		else if (twoDEffectOperator == 6)
		{	
			// rainbow edges kinda cewl lol
			float kernel[9] = float[](
					2.0, 1.0, 2.0,
					1.0, -16.0, 1.0,
					2.0, 1.0, 2.0
			);
			vec3 sampleTex[9];
			for (int i = 0; i < 9; i++)
			{
				sampleTex[i] = vec3(texture(texLightPassColourBuffer, UVlookup + offsets[i]));
			}
			vec3 col = vec3(0.0);
			for (int i = 0; i < 9; i++)
				col += sampleTex[i] * kernel[i];

			sampleColour = col;
		}
		else if (twoDEffectOperator == 7)
		{
			sampleColour = texture( texLightPassColourBuffer, UVlookup ).rgb;
			// idk, make it kinda red and dim the other colours?
			sampleColour.r *= 2.0;
			sampleColour.g /= 2.0;
			sampleColour.b /= 2.0;
		}
		else if (twoDEffectOperator == 8)
		{	// I really want to be able to use a second texture...
//			sampleColour = texture( texLightPassColourBuffer, UVlookup ).rgb
//			- texture (texScope, UVlookup).rgb;

//			sampleColour = texture (texScope, UVlookup).rgb
//			+ texture( texLightPassColourBuffer, UVlookup ).rgb;
			
			// I really need to stop doing this at 2AM LOL, this was much easier to figure out the next day
			// This adds the black parts of the scope texture instead of the scene image
			vec3 texColour = texture (texScope, UVlookup).rgb;
			float average = (texColour.r + texColour.g + texColour.b) / 3.0;
			if (average > 0.3)
			{
				sampleColour = texture( texLightPassColourBuffer, UVlookup ).rgb;
			}
			else
			{
				sampleColour = texture(texScope, UVlookup).rgb;
			}
		}
		else if (twoDEffectOperator == 9)
		{	// overlay the window texture at 0.25 opacity
			sampleColour =  ( texture ( texLightPassColourBuffer, UVlookup ).rgb * 0.75) + 
							( texture ( texWindow, UVlookup ).rgb * 0.25);
		}
		else if (twoDEffectOperator == 10)
		{	// nothing?  puts the no texture texture over the screen.  (I don't remember writing this, but whatever I guess lol)
			vec2 UV2lookup;
			UV2lookup.x = gl_FragCoord.x / 10.0f;	// Width
			UV2lookup.y = gl_FragCoord.y / 10.0f;	// Height
			sampleColour = ( texture ( tex2LightPassColourBuffer, UVlookup ).rgb );
		}
		else
		{
			sampleColour = texture( texLightPassColourBuffer, UVlookup ).rgb;
		}

		//gl_FragCoord is the final screen coordinate for THIS pixel
		
		
		
		
		pixelOutputFragColour.rgb = sampleColour.rgb;
		pixelOutputFragColour.a = 1.0f;

		//vec3 fragColour = pixelOutputFragColour.rgb;

		vec3 bloomColour = texture(tex2LightPassColourBuffer, UVlookup).rgb;

		if (useBloom)
		{
		//vec3 bloomColour = texture (tex2LightPassColourBuffer, UVlookup ).rgb;
		//vec3 bloomColour = texture (tex2VertexMaterialColour, UVlookup ).rgb;
			//vec3 bloomColour = texture (tex2LightPassColourBuffer, UVlookup ).rgb;
			//vec3 bloomColour = texture (texBloomBrightness, UVlookup ).rgb;
			pixelOutputFragColour.rgb += bloomColour * 0.3;	// bloomWeight
//			pixelOutputFragColour.rgb += texture (tex2LightPassColourBuffer, UVlookup ).rgb;
//			vec3 result = vec3(1.0) - exp(-pixelOutputFragColour.rgb * exposure);
//			result = pow(result, vec3(1.0 / 2.2));
//			pixelOutputFragColour = vec4(result, 1.0);
			//pixelOutputFragColour.rgb = texture (tex2LightPassColourBuffer, UVlookup).rgb; 
			//pixelOutputFragColour.rgb = texture (texBloomBrightness, UVlookup).rgb; 
		}

		//pixelOutputFragColour.rgb = texture(tex2LightPassColourBuffer, UVlookup).rgb;
		//pixelOutputFragColour.rgb = texture(tex2VertexMaterialColour, UVlookup).rgb;
		//pixelOutputFragColour.rgb = texture(texBloomBrightness, UVlookup).rgb;

		// Early exit
		return;
	}

//    ____       _      _         _      _    _                                        
//   |___ \  _  | |    (_)  __ _ | |__  | |_ (_) _ __    __ _   _ __    __ _  ___  ___ 
//     __) |(_) | |    | | / _` || '_ \ | __|| || '_ \  / _` | | '_ \  / _` |/ __|/ __|
//    / __/  _  | |___ | || (_| || | | || |_ | || | | || (_| | | |_) || (_| |\__ \\__ \
//   |_____|(_) |_____||_| \__, ||_| |_| \__||_||_| |_| \__, | | .__/  \__,_||___/|___/
//                         |___/                        |___/  |_|                     
	// Apply lighting to G-Buffer
	if ( renderPassNumber == PASS_2_LIGHT_PASS )
	{
		// Now we do the light pass.
		// The big difference is we are reading from the previous textures 
		//	we wrote to in the 1st pass (that wrote to the G-Buffer)

		// Get the UV for this pixel
		vec2 UVlookup;
		UVlookup.x = gl_FragCoord.x / screenWidthHeight.x;	// Width
		UVlookup.y = gl_FragCoord.y / screenWidthHeight.y;	// Height
		
		//	vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
        //                              vec3 vertexWorldPos, vec4 vertexSpecular );
		
		vec4 vertDiffuse = 		 texture( texVertexMaterialColour, UVlookup ).rgba;
		vec4 vertWorldPosition = texture( texVertexWorldPos,       UVlookup ).rgba;
		
		// Is this pixel being lit (calculating light calculation)?
		if ( vertWorldPosition.w == G_BUFFER_OBJECT_NOT_LIT )
		{	
			// Nope. 
			pixelOutputFragColour.rgb = vertDiffuse.rgb;
			pixelOutputFragColour.a = 1.0f;
			// Early exit
			return;
		}
		
		// Otherwise, we ARE lighting this pixel
		// So get the additional info we need for lighting
		vec4 vertNormal = 		 texture( texVertexNormal,         UVlookup ).rgba;
		vec4 vertSpecular = 	 texture( texVertexSpecular,       UVlookup ).rgba;

		//	vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
        //                              vec3 vertexWorldPos, vec4 vertexSpecular );
		//vertSpecular.rgb *= 0.0001f;
		//vertSpecular.rgb += vec3(1.0f);
		
		vertNormal.xyz = normalize(vertNormal.xyz);

		if (textureOperator == 4)
		{
			vec4 normalColour = texture2D ( texture_02, fUVx2.xy, 1.0 ) * texture2DRatios0to3.z;
			// https://learnopengl.com/Advanced-Lighting/Normal-Mapping
			//vec3 normal = vec3(2.0*(normalColour - 0.5));
			vec3 normal = vec3(normalColour * 2.0 - 1.0);
			//normal += fVertWorldLocation.xyz + fNormal.xyz;

			//vertNormal.xyz = normalize(vertNormal.xyz);
			pixelOutputFragColour = calcualteLightContrib( vertDiffuse.rgb,	
		                                               normal.xyz, 		
		                                               vertWorldPosition.xyz,	
		                                               vertSpecular.rgba );

			pixelOutputFragColour.a = 1.0f;

			//vec4 normalColour = texture2D ( texture_02, fUVx2.xy, 1.0 ) * texture2DRatios0to3.z;
//			// https://learnopengl.com/Advanced-Lighting/Normal-Mapping
//			vec3 normal = vec3(2.0*(normalColour - 0.5));
//			//normal += fVertWorldLocation.xyz + fNormal.xyz;
//			
//		
//			// Used for drawing "debug" objects (usually wireframe)
//			if ( bDontLightObject )
//			{
//				pixelOutputFragColour = vertexDiffuseColour;
//				// Early exit from shader
//				return;
//			}
//
//			//pixelColour.rgb = normal;		// was testing to make sure the normal colour was being put through, it would've helped if, y'know, I was setting the vertexDiffuseColour to anything lmao
//	
//	//		vec4 outColour = calcualteLightContrib( vertexDiffuseColour.rgb,		
//	//												fNormal.xyz, 										// Normal at the vertex (in world coords)
//	//												fVertWorldLocation.xyz,							// Vertex WORLD position
//	//												wholeObjectSpecularColour.rgba );
//
//			vec4 outColour = calcualteLightContrib( vertexDiffuseColour.rgb,
//													normal,
//													fVertWorldLocation.xyz,
//													wholeObjectSpecularColour.rgba );
//											
//			pixelOutputFragColour = outColour;
//	
//			// Set the alpha value
//			// 0.0 clear
//			// 1.0 solid
//			pixelOutputFragColour.a = 1.0f;
//			pixelOutputFragColour.a = wholeObjectAlphaTransparency;
//			return;

			//return;
		}
		else
		{
			pixelOutputFragColour = calcualteLightContrib( vertDiffuse.rgb,	
														   vertNormal.xyz, 		
														   vertWorldPosition.xyz,	
														   vertSpecular.rgba );
		}											   

																
		pixelOutputFragColour.a = 1.0f;

//		pixelOutputFragColour.rgb *= 0.0001f;	
//		pixelOutputFragColour.rgb += vertNormal.rgb;
												
		return;
	}

	// Else, it's:
	// Renders only geometry to G-Buffer
	// const uint PASS_1_G_BUFFER_PASS = 1;	

	// Set the alpha value: 0.0 = clear, 1.0 = solid
	pixelOutputFragColour.a = wholeObjectAlphaTransparency;		
	pixelOutputFragColour.rgb = vec3(0.0f, 0.0f, 0.0f);
	
	// Set the other FBO outputs
	pixelOutputMaterialColour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	pixelOutputNormal = vec4(fNormal.xyz, 1.0f);
	pixelOutputWorldPos.xyz = vec3(0.0f, 0.0f, 0.0f);
	pixelOutputWorldPos.w = G_BUFFER_LIT;
	pixelOutputSpecular = vec4(0.0f, 0.0f, 0.0f, 1.0f);

	// If face normals are being generated from the geometry shader, 
	//	then this is true, and the colours are taken from the 
	if ( int(fDebugColourOverride.w) == 1 )
	{
	// OG
//		pixelOutputFragColour = fDebugColourOverride;
//		return;	
		// Original colour buffer output
		//pixelOutputFragColour = fDebugColourOverride;
		
		// Now G-Buffer output:
		pixelOutputMaterialColour.rgb = fDebugColourOverride.rgb;
		// Don't care about normals (pixelOutputNormal)
		pixelOutputWorldPos.xyz = fVertWorldLocation.xyz;
		// This ISN'T lit, so set w = 
		pixelOutputWorldPos.w = G_BUFFER_OBJECT_NOT_LIT;
		// Not lit, so ignore specular, too (pixelOutputSpecular)	

		return;	
	}

	// Black hole discard example
//	vec3 blackHoleLocationXYZ = vec3(0.0f, 0.0f, 0.0f);
//	
//	if ( distance(fVertWorldLocation.xyz, blackHoleLocationXYZ) < blackHoleSize )
//	{
//		discard;
//	}

if ( bIsSkyBox )
	{
		//pixelColour.a = 1.0f;
		// For some reason if the cube map isn't actually set to
		// a texture unit it returns black when combined with other cubemaps
//		pixelColour.rgb = 
//		( texture( cubeMap_00, fNormal.xyz ).rgb * cubeMap_Ratios0to3.x ) +
//		( texture( cubeMap_01, fNormal.xyz ).rgb * cubeMap_Ratios0to3.y ) +
//		( texture( cubeMap_02, fNormal.xyz ).rgb * cubeMap_Ratios0to3.z ) +
//		( texture( cubeMap_03, fNormal.xyz ).rgb * cubeMap_Ratios0to3.w );
		
		// Feeney uses Material Colour
		if (cubeMap_Ratios0to3.x > 0.0f)
		{
			//pixelOutputFragColour.rgb += ( texture( cubeMap_00, fNormal.xyz ).rgb * cubeMap_Ratios0to3.x );
			pixelOutputMaterialColour.rgb += ( texture( cubeMap_00, fNormal.xyz ).rgb * cubeMap_Ratios0to3.x );
		}
		if (cubeMap_Ratios0to3.y > 0.0f)
		{
			//pixelOutputFragColour.rgb += ( texture( cubeMap_01, fNormal.xyz ).rgb * cubeMap_Ratios0to3.y );
			pixelOutputMaterialColour.rgb += ( texture( cubeMap_01, fNormal.xyz ).rgb * cubeMap_Ratios0to3.y );
		}
		if (cubeMap_Ratios0to3.z > 0.0f)
		{
			//pixelOutputFragColour.rgb += ( texture( cubeMap_02, fNormal.xyz ).rgb * cubeMap_Ratios0to3.z );
			pixelOutputMaterialColour.rgb += ( texture( cubeMap_02, fNormal.xyz ).rgb * cubeMap_Ratios0to3.z );
		}
		if (cubeMap_Ratios0to3.w > 0.0f)
		{
			//pixelOutputFragColour.rgb += ( texture( cubeMap_03, fNormal.xyz ).rgb * cubeMap_Ratios0to3.w );
			pixelOutputMaterialColour.rgb += ( texture( cubeMap_03, fNormal.xyz ).rgb * cubeMap_Ratios0to3.w );
		}

		// Don't care about normals (pixelOutputNormal)
		pixelOutputWorldPos.xyz = fVertWorldLocation.xyz;
		// This ISN'T lit, so set w = 
		pixelOutputWorldPos.w = G_BUFFER_OBJECT_NOT_LIT;
		// Not lit, so ignore specular, too (pixelOutputSpecular)

		return;
	}



	if ( bIsImposter )
	{
		// Test for close to "black". 
		// If "black enough", then call discard.
		vec3 sampleColour;
		sampleColour.rgb =  	
			(texture( texture_00, fUVx2.xy ).rgb * texture2DRatios0to3.x)  + 
			(texture( texture_01, fUVx2.xy ).rgb * texture2DRatios0to3.y)  + 
			(texture( texture_02, fUVx2.xy ).rgb * texture2DRatios0to3.z)  + 
			(texture( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w);
		
		// Is it "black enough"? 
		float greyscaleValue = (sampleColour.r + sampleColour.g + sampleColour.b) / 3.0f;
		const float BLACK_ENOUGH_THRESHOLD = 0.1f;
		if ( greyscaleValue < BLACK_ENOUGH_THRESHOLD )
		{
			discard;
		}
		// This was my colour change thing, gunna have to redo it if Feeney's things are right
		// Slight change to the imposter.  I want my torches to be red, but the texture I'm using is mostly blue!
		// So, I'm going to change which is which
		//pixelColour.rgb = sampleColour.rgb;
//		pixelOutputFragColour.r = sampleColour.b;
//		pixelOutputFragColour.g = sampleColour.g;
//		pixelOutputFragColour.b = sampleColour.r;
//		// 
//		// else use the greyscaleValue as the alpha value 
//		// 	(closer to black = more transparent)
//		// Since the flame is blue coloured, we are only going to use blue for the alpha
//		// And this will be the same since the texture itself is still blue
//		pixelOutputFragColour.a = greyscaleValue * 1.25f; 	//(sampleColour.b / 10.0f);
//	
//		To re-create my red instead of blue thing this is probably what I'm changing (since it uses the sampleColour)
		//pixelOutputMaterialColour.rgb = sampleColour.rgb;
		pixelOutputMaterialColour.r = sampleColour.b;
		pixelOutputMaterialColour.g = sampleColour.g;
		pixelOutputMaterialColour.b = sampleColour.r;
		// NOTE: This isn't really going to work because aplha transparency won't work with deferred.
		pixelOutputMaterialColour.a = greyscaleValue * 1.25f;
		
		pixelOutputNormal = fNormal;
		pixelOutputWorldPos = fVertWorldLocation;
		pixelOutputSpecular = wholeObjectSpecularColour;
	
		// Early exit, no lighting
		pixelOutputWorldPos.w = G_BUFFER_OBJECT_NOT_LIT;


		// Early exit, no lighting
		return;
	}

	// Feeney has this commented out
	if (bDiscardTransparencyWindowsOn)
	{
		// Note I'm only sampling from red because I just want
		// to see if ti's "black-ish" coloured...
		vec3 vec3DisSample = texture ( discardTexture, fUVx2.xy ).rgb;
		float discardSample = ( vec3DisSample.r + vec3DisSample.g + vec3DisSample.b )/3.0f;
		//
	//
	
		if (textureOperator == 1)
		{
			if ( discardSample > 0.99f )	// reversed, discarding white
			{	// "black enough"
				// DON'T even draw the pixel here
				// The fragment shader simply stops here
				discard;
			}
		}
		else if (textureOperator == 2)
		{
			if (discardSample > 0.7)
			{
				discard;
			}
		}

		// For the mod ship, this "teal" colour is used for the window
		if ((vec3DisSample.r > 0.55 && vec3DisSample.r < 0.65) && 
			(vec3DisSample.g > 0.8 && vec3DisSample.g < 0.9) &&
			(vec3DisSample.b > 0.87 && vec3DisSample.b < 0.97))
		{
			discard;	
		}

	}

	
	// Copy model vertex colours?
	vec4 vertexDiffuseColour = fVertexColour; //vec4(0.0f, 0.0f, 0.0f, 1.0f);

	
	// Use model vertex colours or not?
	if ( bUseWholeObjectDiffuseColour )
	{
		vertexDiffuseColour = wholeObjectDiffuseColour;
	}
	
	// Use debug colour?
	if ( bUseDebugColour )
	{
		// Overwrite the vertex colour with this debug colour
		vertexDiffuseColour = objectDebugColour;	
	}

	// I moved it to before the bDontLight so that textures should be set
	// Use the texture values as the "diffuse" colour


	if ( (bUseWholeObjectDiffuseColour == false) && 
	     (bUseDebugColour == false) )
	{
		if (textureOperator == 4)
		{
			vertexDiffuseColour.rgb = texture( texture_01, fUVx2.xy ).rgb * texture2DRatios0to3.y;

			vec3 normalColour = texture2D ( texture_02, fUVx2.xy, 1.0 ).rgb * texture2DRatios0to3.z;
			// https://learnopengl.com/Advanced-Lighting/Normal-Mapping
			//vec3 normal = vec3(2.0*(normalColour - 0.5));
			vec3 normal = normalize( normalColour * 2.0 - 1.0 );
			//vec3 normal = vec3(normalColour.x * 2.0 - 1.0, normalColour.y * 2.0 - 1.0, normalColour.z * 2.0 -1.0);
			//normal += fVertWorldLocation.xyz + fNormal.xyz;

			//vertNormal.xyz = normalize(vertNormal.xyz);
			//pixelOutputFragColour = calcualteLightContrib( vertexDiffuseColour.rgb,	
			vertexDiffuseColour = calcualteLightContrib( vertexDiffuseColour.rgb,	
		                                               normal.xyz, 		
		                                               fVertWorldLocation.xyz,	
		                                               wholeObjectSpecularColour.rgba );

			//pixelOutputFragColour.a = 1.0f;
			vertexDiffuseColour.a = 1.0f;

			// I think the rest is for lighting/specular so that would happen during the lighting pass
		// old DFK
			//vertexDiffuseColour.rgb =	texture ( texture_01, fUVx2.xy ).rgb * texture2DRatios0to3.y;// +
//										//texture ( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w;		// this one line cost me about 3 hours...
//			vec4 normalColour = texture2D ( texture_02, fUVx2.xy, 1.0 ) * texture2DRatios0to3.z;
//			// https://learnopengl.com/Advanced-Lighting/Normal-Mapping
//			vec3 normal = vec3(2.0*(normalColour - 0.5));
//			//normal += fVertWorldLocation.xyz + fNormal.xyz;
//			
//		
//			// Used for drawing "debug" objects (usually wireframe)
//			if ( bDontLightObject )
//			{
//				pixelOutputFragColour = vertexDiffuseColour;
//				// Early exit from shader
//				return;
//			}
//
//			//pixelColour.rgb = normal;		// was testing to make sure the normal colour was being put through, it would've helped if, y'know, I was setting the vertexDiffuseColour to anything lmao
//	
//	//		vec4 outColour = calcualteLightContrib( vertexDiffuseColour.rgb,		
//	//												fNormal.xyz, 										// Normal at the vertex (in world coords)
//	//												fVertWorldLocation.xyz,							// Vertex WORLD position
//	//												wholeObjectSpecularColour.rgba );
//
//			vec4 outColour = calcualteLightContrib( vertexDiffuseColour.rgb,
//													normal,
//													fVertWorldLocation.xyz,
//													wholeObjectSpecularColour.rgba );
//											
//			pixelOutputFragColour = outColour;
//	
//			// Set the alpha value
//			// 0.0 clear
//			// 1.0 solid
//			pixelOutputFragColour.a = 1.0f;
//			pixelOutputFragColour.a = wholeObjectAlphaTransparency;
//			return;
		}
		// https://learnopengl.com/Advanced-OpenGL/Cubemaps
		else if (textureOperator == 5)
		{	// reflection
			vec3 I = normalize(fVertWorldLocation.xyz - eyeLocation.xyz);
			vec3 R = reflect(I, normalize(fNormal.xyz));

			vertexDiffuseColour.rgb =
							(texture( cubeMap_00, R ).rgb * cubeMap_Ratios0to3.x)  + 
							(texture( cubeMap_01, R ).rgb * cubeMap_Ratios0to3.y)  + 
							(texture( cubeMap_02, R ).rgb * cubeMap_Ratios0to3.z)  + 
							(texture( cubeMap_03, R ).rgb * cubeMap_Ratios0to3.w);

			vertexDiffuseColour.a = 1.0f;

		}
		else if (textureOperator == 6)
		{	// refractions
		// values for the ratio could be sent in as variables by the object using it
			float ratio = 1.00 / 1.52;		// different materials have different refractive index, this is air to glass
			vec3 I = normalize(fVertWorldLocation.xyz - eyeLocation.xyz);
			vec3 R = refract(I, normalize(fNormal.xyz), ratio);

			vertexDiffuseColour.rgb =
							(texture( cubeMap_00, R ).rgb * cubeMap_Ratios0to3.x)  + 
							(texture( cubeMap_01, R ).rgb * cubeMap_Ratios0to3.y)  + 
							(texture( cubeMap_02, R ).rgb * cubeMap_Ratios0to3.z)  + 
							(texture( cubeMap_03, R ).rgb * cubeMap_Ratios0to3.w);

			vertexDiffuseColour.a = 1.0f;
		} //might add a reflect/refract
		else if (textureOperator == 10)
		{	// this was how I figured out how to do the FBO stuff... too bad I only needed 2 and have to use these for the texture effects lol
//			vec2 UVlookup;
//			UVlookup.x = gl_FragCoord.x / screenWidthHeight.x;	// Width
//			UVlookup.y = gl_FragCoord.y / screenWidthHeight.y;	// Height
//			//vec3 sampleColour = texture( tex2LightPassColourBuffer, fUVx2.xy ).rgb;
//			//vec3 sampleColour = texture( tex2VertexMaterialColour, UVlookup ).rgb;
//			//vec3 sampleColour = texture( tex2VertexMaterialColour, fUVx2.xy ).rgb;
//			//vec3 sampleColour = texture( tex2VertexNormal, fUVx2.xy ).rgb;
//			//vec3 sampleColour = texture( tex2VertexSpecular, fUVx2.xy ).rgb;
//			//vec3 sampleColour = texture( tex2VertexWorldPos, fUVx2.xy ).rgb;
//			//vec3 sampleColour = texture( texLightPassColourBuffer, fUVx2.xy ).rgb;
//
//			vertexDiffuseColour.rgb = 	
//					(texture( screenTex1VertexMaterialColour, fUVx2.xy ).rgb * texture2DRatios0to3.x);
//					///(texture( tex2VertexMaterialColour, fUVx2.xy ).rgb * texture2DRatios0to3.x);
//
//			//vertexDiffuseColour.rgb = sampleColour.rgb;
//			//vertexDiffuseColour.rgb = new vec3(1.0f, 0.0f, 1.0f);
//			vertexDiffuseColour.a = 1.0f;
//
//			pixelOutputNormal = vec4(normalize(fNormal.xyz), 1.0f);
//			pixelOutputMaterialColour = vec4(vertexDiffuseColour.rgb, 1.0f);
//			pixelOutputWorldPos = vec4(fVertWorldLocation.xyz, 1.0f);
//			pixelOutputWorldPos.w = G_BUFFER_LIT;
//			pixelOutputSpecular = wholeObjectSpecularColour.rgba;			// = 4;	
//		
//			// Early exit
//			return;

			vertexDiffuseColour.rgb = (								
					( texture ( texture_00, vec2(fUVx2.x, fUVx2.y + flickerOffset.x) ) * texture2DRatios0to3.x) +	// default no texture
					( texture ( texture_01, vec2(fUVx2.x, fUVx2.y + flickerOffset.x) ) * texture2DRatios0to3.y) +	// first of 3 screens
					( texture ( texture_02, vec2(fUVx2.x, fUVx2.y + flickerOffset.x) ) * texture2DRatios0to3.z) +	// second of 3 screens
					( texture ( texture_03, vec2(fUVx2.x, fUVx2.y + flickerOffset.x) ) * texture2DRatios0to3.w) +	// third of 3 screens
					( texture ( texture_04, vec2(fUVx2.x + cam1StaticOffset.x, fUVx2.y + cam1StaticOffset.x) ) * texture2DRatios4to7.x)		// static texture
					).rgb;	

			
			vertexDiffuseColour.a = 1.0f;

		}
		else if (textureOperator == 11)
		{	// I don't think I need 11, it should just be a repeat of 10 but idk, might be easier to do the flicker by passing a vec2 and changing the offset in C, then 10 would use x and 11 y
//			vertexDiffuseColour.rgb = 	
//					(texture( screenTex2VertexMaterialColour, fUVx2.xy ).rgb * texture2DRatios0to3.x);
//					///(texture( tex2VertexMaterialColour, fUVx2.xy ).rgb * texture2DRatios0to3.x);
//
//			vertexDiffuseColour.a = 1.0f;

			vertexDiffuseColour.rgb = (								
					( texture ( texture_00, vec2(fUVx2.x, fUVx2.y + flickerOffset.y) ) * texture2DRatios0to3.x) +	// default no texture
					( texture ( texture_01, vec2(fUVx2.x, fUVx2.y + flickerOffset.y) ) * texture2DRatios0to3.y) +	// first of 3 screens
					( texture ( texture_02, vec2(fUVx2.x, fUVx2.y + flickerOffset.y) ) * texture2DRatios0to3.z) +	// second of 3 screens
					( texture ( texture_03, vec2(fUVx2.x, fUVx2.y + flickerOffset.y) ) * texture2DRatios0to3.w) +	// third of 3 screens
					( texture ( texture_04, vec2(fUVx2.x + cam2StaticOffset.x , fUVx2.y + cam2StaticOffset.y) ) * texture2DRatios4to7.x)		// static texture
					).rgb;	

			
			vertexDiffuseColour.a = 1.0f;

		}
		else if (textureOperator == 12)
		{	// FBO textures
			vertexDiffuseColour.rgb = 	
					(texture( screenTex3VertexMaterialColour, fUVx2.xy ).rgb * texture2DRatios0to3.y);
					///(texture( tex2VertexMaterialColour, fUVx2.xy ).rgb * texture2DRatios0to3.x);

			vertexDiffuseColour.a = 1.0f;

		}
		else if (textureOperator == 13)
		{	// FBO textures
			vertexDiffuseColour.rgb = 	
					(texture( screenTex4VertexMaterialColour, fUVx2.xy ).rgb * texture2DRatios0to3.y);
					///(texture( tex2VertexMaterialColour, fUVx2.xy ).rgb * texture2DRatios0to3.x);

			vertexDiffuseColour.a = 1.0f;

		}
		else if (textureOperator == 14)
		{	// FBO textures
			vertexDiffuseColour.rgb = ( 	
					( texture ( screenTex1VertexMaterialColour,  vec2(fUVx2.x, fUVx2.y + flickerOffset.x) ) * texture2DRatios0to3.y) +
					( texture ( texture_04, vec2(fUVx2.x + cam1StaticOffset.x, fUVx2.y + cam1StaticOffset.x) ) * texture2DRatios4to7.x)
					).rgb;
					///(texture( tex2VertexMaterialColour, fUVx2.xy ).rgb * texture2DRatios0to3.x);

			vertexDiffuseColour.a = 1.0f;

		}
		else if (textureOperator == 20)
		{	// Stencil "Buffer"
			vertexDiffuseColour = stencilColour;
		}
		else
		{
			vertexDiffuseColour.rgb = 	
					(texture( texture_00, fUVx2.xy ).rgb * texture2DRatios0to3.x)  + 
					(texture( texture_01, fUVx2.xy ).rgb * texture2DRatios0to3.y)  + 
					(texture( texture_02, fUVx2.xy ).rgb * texture2DRatios0to3.z)  + 
					(texture( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w);
				// + etc... the other 4 texture units
		}
	// commented out the operator if check for now to test Feeney's thing (just adding textures)
		// Makes this "black" but not quite...
//		vertexDiffuseColour.rgb *= 0.0001f;
//	
//		if (textureOperator == 0)	// addition
//		{
//			vertexDiffuseColour.rgb =	texture ( texture_00, fUVx2.xy ).rgb * texture2DRatios0to3.x + 
//										texture ( texture_01, fUVx2.xy ).rgb * texture2DRatios0to3.y + 
//										texture ( texture_02, fUVx2.xy ).rgb * texture2DRatios0to3.z + 
//										texture ( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w;
//										// + etc... the other 4 texture units
//		}
//		else if (textureOperator == 1)	// subtraction
//		{
//			// texture2DRatios0to3.x is 0 if there are no other textures, so we need to ignore it if it's 0
//			if ( texture2DRatios0to3.x != 0.0f )
//			{
//				vertexDiffuseColour.rgb =	texture ( texture_00, fUVx2.xy ).rgb * texture2DRatios0to3.x -		//* texture2DRatios0to3.x
//											texture ( texture_01, fUVx2.xy ).rgb * texture2DRatios0to3.y -		//* texture2DRatios0to3.y
//											texture ( texture_02, fUVx2.xy ).rgb * texture2DRatios0to3.z -		//* texture2DRatios0to3.z
//											texture ( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w;
//			}
//			else if ( texture2DRatios0to3.y != 0.0f )
//			{
//				vertexDiffuseColour.rgb =	texture ( texture_01, fUVx2.xy ).rgb * texture2DRatios0to3.y -		//* texture2DRatios0to3.y
//											texture ( texture_02, fUVx2.xy ).rgb * texture2DRatios0to3.z -		//* texture2DRatios0to3.z
//											texture ( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w;
//			}
//			else if ( texture2DRatios0to3.z != 0.0f )
//			{
//				vertexDiffuseColour.rgb =	texture ( texture_02, fUVx2.xy ).rgb * texture2DRatios0to3.z -		//* texture2DRatios0to3.z
//											texture ( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w;
//			}
//			else if ( texture2DRatios0to3.w != 0.0f )
//			{
//				vertexDiffuseColour.rgb =	texture ( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w;
//			}
//		}
//		else if (textureOperator == 2)	// multiplication
//		{
//			// again, 0 will just make everything black, so go one by one
//			if ( texture2DRatios0to3.x != 0.0f )
//			{
//				vertexDiffuseColour.rgb =	texture ( texture_00, fUVx2.xy ).rgb * texture2DRatios0to3.x;
//
//				if ( texture2DRatios0to3.y != 0.0f )
//				{
//					vertexDiffuseColour.rgb *=	texture ( texture_01, fUVx2.xy ).rgb * texture2DRatios0to3.y;
//				}
//				if ( texture2DRatios0to3.z != 0.0f )
//				{
//					vertexDiffuseColour.rgb *=	texture ( texture_02, fUVx2.xy ).rgb * texture2DRatios0to3.z; 
//				}
//				if ( texture2DRatios0to3.w != 0.0f )
//				{
//					vertexDiffuseColour.rgb *=	texture ( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w;
//				}
//			}
//			else if ( texture2DRatios0to3.y != 0.0f )
//			{
//				vertexDiffuseColour.rgb =	texture ( texture_01, fUVx2.xy ).rgb * texture2DRatios0to3.y;
//				if ( texture2DRatios0to3.z != 0.0f )
//				{
//					vertexDiffuseColour.rgb *=	texture ( texture_02, fUVx2.xy ).rgb * texture2DRatios0to3.z; 
//				}
//				if ( texture2DRatios0to3.w != 0.0f )
//				{
//					vertexDiffuseColour.rgb *=	texture ( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w;
//				}
//			}
//			else if ( texture2DRatios0to3.z != 0.0f )
//			{
//				vertexDiffuseColour.rgb =	texture ( texture_02, fUVx2.xy ).rgb * texture2DRatios0to3.z; 
//				if ( texture2DRatios0to3.w != 0.0f )
//				{
//					vertexDiffuseColour.rgb *=	texture ( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w;
//				}
//			}
//			else if ( texture2DRatios0to3.w != 0.0f )
//			{
//				vertexDiffuseColour.rgb =	texture ( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w;
//			}
//		}
//		else if (textureOperator == 3)	// division
//		{
//			if ( texture2DRatios0to3.x != 0.0f )
//			{
//				vertexDiffuseColour.rgb =	texture ( texture_00, fUVx2.xy ).rgb * texture2DRatios0to3.x;
//
//				if ( texture2DRatios0to3.y != 0.0f )
//				{
//					vertexDiffuseColour.rgb /=	texture ( texture_01, fUVx2.xy ).rgb * texture2DRatios0to3.y;
//				}
//				if ( texture2DRatios0to3.z != 0.0f )
//				{
//					vertexDiffuseColour.rgb /=	texture ( texture_02, fUVx2.xy ).rgb * texture2DRatios0to3.z; 
//				}
//				if ( texture2DRatios0to3.w != 0.0f )
//				{
//					vertexDiffuseColour.rgb /=	texture ( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w;
//				}
//			}
//			else if ( texture2DRatios0to3.y != 0.0f )
//			{
//				vertexDiffuseColour.rgb =	texture ( texture_01, fUVx2.xy ).rgb * texture2DRatios0to3.y;
//				if ( texture2DRatios0to3.z != 0.0f )
//				{
//					vertexDiffuseColour.rgb /=	texture ( texture_02, fUVx2.xy ).rgb * texture2DRatios0to3.z; 
//				}
//				if ( texture2DRatios0to3.w != 0.0f )
//				{
//					vertexDiffuseColour.rgb /=	texture ( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w;
//				}
//			}
//			else if ( texture2DRatios0to3.z != 0.0f )
//			{
//				vertexDiffuseColour.rgb =	texture ( texture_02, fUVx2.xy ).rgb * texture2DRatios0to3.z; 
//				if ( texture2DRatios0to3.w != 0.0f )
//				{
//					vertexDiffuseColour.rgb /=	texture ( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w;
//				}
//			}
//			else if ( texture2DRatios0to3.w != 0.0f )
//			{
//				vertexDiffuseColour.rgb =	texture ( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w;
//			}
//		}
//		else if (textureOperator == 4)
//		{	// DFK, use the normal texture map and SmMetAO texture to change things around :)
//
//			vertexDiffuseColour.rgb =	texture ( texture_01, fUVx2.xy ).rgb * texture2DRatios0to3.y;// +
//										//texture ( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w;		// this one line cost me about 3 hours...
//			vec4 normalColour = texture2D ( texture_02, fUVx2.xy, 1.0 ) * texture2DRatios0to3.z;
//			// https://learnopengl.com/Advanced-Lighting/Normal-Mapping
//			vec3 normal = vec3(2.0*(normalColour - 0.5));
//			//normal += fVertWorldLocation.xyz + fNormal.xyz;
//			
//		
//			// Used for drawing "debug" objects (usually wireframe)
//			if ( bDontLightObject )
//			{
//				pixelOutputFragColour = vertexDiffuseColour;
//				// Early exit from shader
//				return;
//			}
//
//			//pixelColour.rgb = normal;		// was testing to make sure the normal colour was being put through, it would've helped if, y'know, I was setting the vertexDiffuseColour to anything lmao
//	
//	//		vec4 outColour = calcualteLightContrib( vertexDiffuseColour.rgb,		
//	//												fNormal.xyz, 										// Normal at the vertex (in world coords)
//	//												fVertWorldLocation.xyz,							// Vertex WORLD position
//	//												wholeObjectSpecularColour.rgba );
//
//			vec4 outColour = calcualteLightContrib( vertexDiffuseColour.rgb,
//													normal,
//													fVertWorldLocation.xyz,
//													wholeObjectSpecularColour.rgba );
//											
//			pixelOutputFragColour = outColour;
//	
//			// Set the alpha value
//			// 0.0 clear
//			// 1.0 solid
//			pixelOutputFragColour.a = 1.0f;
//			pixelOutputFragColour.a = wholeObjectAlphaTransparency;
//			return;
//		}
//		else	// in case nothing is set, just add the textures together
//		{
//			vertexDiffuseColour.rgb =	texture ( texture_00, fUVx2.xy ).rgb * texture2DRatios0to3.x + 
//										texture ( texture_01, fUVx2.xy ).rgb * texture2DRatios0to3.y + 
//										texture ( texture_02, fUVx2.xy ).rgb * texture2DRatios0to3.z + 
//										texture ( texture_03, fUVx2.xy ).rgb * texture2DRatios0to3.w;
//										// + etc... the other 4 texture units
//		}

	}
	

	
	// Multi passing doesn't run light calculations here, it's all done on it's own pass
	// Used for drawing "debug" objects (usually wireframe)
//	if ( bDontLightObject )
//	{
//		// Early exit from shader
//		pixelOutputFragColour.rgb = vertexDiffuseColour.rgb;
//		pixelOutputFragColour.a = wholeObjectAlphaTransparency;
//		return;
//	}
//
//	
//	
//	
//	vec4 outColour = calcualteLightContrib( vertexDiffuseColour.rgb,		
//	                                        fNormal.xyz, 		// Normal at the vertex (in world coords)
//                                            fVertWorldLocation.xyz,	// Vertex WORLD position
//											wholeObjectSpecularColour.rgba );
//											
//	pixelOutputFragColour = outColour;
	

	// Output the other things for the G-Buffer:
	pixelOutputNormal = vec4(normalize(fNormal.xyz), 1.0f);
	//pixelOutputMaterialColour = vec4(vertexDiffuseColour.rgb, 1.0f);
	pixelOutputMaterialColour = vec4(vertexDiffuseColour.rgb, wholeObjectAlphaTransparency);
	pixelOutputWorldPos = vec4(fVertWorldLocation.xyz, 1.0f);
	pixelOutputWorldPos.w = G_BUFFER_LIT;
	pixelOutputSpecular = wholeObjectSpecularColour.rgba;			// = 4;	
	
	
	// Set the alpha value
	// 0.0 clear
	// 1.0 solid
	//pixelColour.a = 1.0f;
	//pixelOutputFragColour.a = wholeObjectAlphaTransparency;
};


// Calculates the colour of the vertex based on the lighting and vertex information:
vec4 calcualteLightContrib( vec3 vertexMaterialColour, vec3 vertexNormal, 
                            vec3 vertexWorldPos, vec4 vertexSpecular )
{
	vec3 norm = normalize(vertexNormal);
	
	vec4 finalObjectColour = vec4( 0.0f, 0.0f, 0.0f, 1.0f );
	
	for ( int index = 0; index < NUMBEROFLIGHTS; index++ )
	{	
		// ********************************************************
		// is light "on"
		if ( theLights[index].param2.x == 0.0f )
		{	// it's off
			continue;
		}
		
		// Cast to an int (note with c'tor)
		int intLightType = int(theLights[index].param1.x);
		
		// We will do the directional light here... 
		// (BEFORE the attenuation, since sunlight has no attenuation, really)
		switch ( intLightType )
		{
		case DIRECTIONAL_LIGHT_TYPE:
			//if ( intLightType == DIRECTIONAL_LIGHT_TYPE )		// = 2
			{
				// This is supposed to simulate sunlight. 
				// SO: 
				// -- There's ONLY direction, no position
				// -- Almost always, there's only 1 of these in a scene
				// Cheapest light to calculate. 

				vec3 lightContrib = theLights[index].diffuse.rgb;
				
				// Get the dot product of the light and normalize
				float dotProduct = dot( -theLights[index].direction.xyz,  
										   normalize(norm.xyz) );	// -1 to 1
				
				dotProduct = max( 0.0f, dotProduct );		// 0 to 1
			
				lightContrib *= dotProduct * 0.7f;		
				
				finalObjectColour.rgb += (vertexMaterialColour.rgb * lightContrib); 
										 //+ (materialSpecular.rgb * lightSpecularContrib.rgb);
				// NOTE: There isn't any attenuation, like with sunlight.
				// (This is part of the reason directional lights are fast to calculate)
				
				//return finalObjectColour;		
			}
			break;
		case POINT_LIGHT_TYPE:
		case SPOT_LIGHT_TYPE:
			{
				// only run the lights that are within 50 units, likely not drawing or drawing low poly models at this distance anyways
				float distFromEye = length(theLights[index].position.xyz - eyeLocation.xyz);
				if ( !bUseAllLights && distFromEye > 50)
				{
					continue;
				}
				// Assume it's a point light 
				// intLightType = 0
				
				// Contribution for this light
				vec3 vLightToVertex = theLights[index].position.xyz - vertexWorldPos.xyz;
				float distanceToLight = length(vLightToVertex);	
				if (distanceToLight > theLights[index].atten.w)
				{
					// Light is too far away
					continue;
				}

				vec3 lightVector = normalize(vLightToVertex);;
				//if ( textureOperator == 4 )
//				if ( textureOperator == 4 )
//				{
//					mat3 transposeTBN = transpose(fTBN);
//					vec3 tangentLightPos = transposeTBN * theLights[index].position.xyz;
//					lightVector = normalize(tangentLightPos - fTangentFragPos);
//					//vLightToVertex = fTangentLightPos - fTangentFragPos;
//				}
//				else
//				{
//					lightVector = normalize(vLightToVertex);
//				}
				
				//float dotProduct = dot(lightVector, vertexNormal.xyz);	 
				float dotProduct = dot(lightVector, norm);	 
				
				dotProduct = max( 0.0f, dotProduct );	
				
				vec3 lightDiffuseContrib = dotProduct * theLights[index].diffuse.rgb;
					

				// Specular 
				vec3 lightSpecularContrib = vec3(0.0f);
					
				//vec3 reflectVector = reflect( -lightVector, normalize(norm.xyz) );
				vec3 reflectVector = reflect( -lightVector, norm.xyz );

				// Get eye or view vector
				// The location of the vertex in the world to your eye
				vec3 eyeVector; 
				//if ( textureOperator == 4 ) 
				if ( textureOperator == 100 ) 
				{
					//eyeVector = normalize( fTangentViewPos - fTangentFragPos );
					eyeVector = normalize( eyeLocation.xyz - fTangentFragPos );
				}
				else
				{
					eyeVector = normalize(eyeLocation.xyz - vertexWorldPos.xyz);
				}

				// To simplify, we are NOT using the light specular value, just the object’s.
				float objectSpecularPower = vertexSpecular.w; 
				
				lightSpecularContrib = pow( max(0.0f, dot( eyeVector, reflectVector) ), objectSpecularPower )
									   * theLights[index].specular.rgb;
									   
				// Attenuation
				float attenuation = 1.0f / 
						( theLights[index].atten.x + 										
						  theLights[index].atten.y * distanceToLight +						
						  theLights[index].atten.z * distanceToLight*distanceToLight );  	
						  
				// total light contribution is Diffuse + Specular
				lightDiffuseContrib *= attenuation;
				lightSpecularContrib *= attenuation;
				
				
				// But is it a spot light
				if ( intLightType == SPOT_LIGHT_TYPE )		// = 1
				{	
				

					// Yes, it's a spotlight
					// Calcualate light vector (light to vertex, in world)
					vec3 vertexToLight = vertexWorldPos.xyz - theLights[index].position.xyz;

					vertexToLight = normalize(vertexToLight);

					float currentLightRayAngle
							= dot( vertexToLight.xyz, theLights[index].direction.xyz );
							
					currentLightRayAngle = max(0.0f, currentLightRayAngle);

					//vec4 param1;	
					// x = lightType, y = inner angle, z = outer angle, w = TBD

					// Is this inside the cone? 
					float outerConeAngleCos = cos(radians(theLights[index].param1.z));
					float innerConeAngleCos = cos(radians(theLights[index].param1.y));
									
					// Is it completely outside of the spot?
					if ( currentLightRayAngle < outerConeAngleCos )
					{
						// Nope. so it's in the dark
						lightDiffuseContrib = vec3(0.0f, 0.0f, 0.0f);
						lightSpecularContrib = vec3(0.0f, 0.0f, 0.0f);
					}
					else if ( currentLightRayAngle < innerConeAngleCos )
					{
						// Angle is between the inner and outer cone
						// (this is called the penumbra of the spot light, by the way)
						// 
						// This blends the brightness from full brightness, near the inner cone
						//	to black, near the outter cone
						float penumbraRatio = (currentLightRayAngle - outerConeAngleCos) / 
											  (innerConeAngleCos - outerConeAngleCos);
											  
						lightDiffuseContrib *= penumbraRatio;
						lightSpecularContrib *= penumbraRatio;
					}//if ( currentLightRayAngle
								
											  
				}// if ( intLightType == 1 )

				// should change this to use the textureOperator
				//if (texture2DRatios0to3.w != 0 && texture(texture_03, fUVx2.xy).r > 0)
				if (textureOperator == 4 && texture2DRatios0to3.w != 0 && texture(texture_03, fUVx2.xy).r > 0)
				{
				// https://learnopengl.com/Advanced-Lighting/SSAO
					float ambientOcclusion = texture(texture_03, fUVx2.xy).r;
					vec3 ambient = vec3(texture2DRatios0to3.w * vertexMaterialColour * ambientOcclusion);

					ambient += (vertexMaterialColour.rgb * lightDiffuseContrib.rgb) + (vertexSpecular.rgb * lightSpecularContrib.rgb);
					finalObjectColour.rgb += ambient;
				}
				else
				{
					finalObjectColour.rgb += (vertexMaterialColour.rgb * lightDiffuseContrib.rgb)
										  + (vertexSpecular.rgb  * lightSpecularContrib.rgb );
				}
								  
			}
			break;
		}//switch ( intLightType )
		
		
	}//for(intindex=0...
	
	finalObjectColour.a = 1.0f;
	
	return finalObjectColour;
}