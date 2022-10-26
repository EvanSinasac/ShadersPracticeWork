#version 420
// GEOMETRY SHADER

layout(triangles)			in;
layout(triangle_strip)		out;
layout(max_vertices = 6)	out;

// Coming from the vertex shader
in vec4 gVertexColour[];			// Triangles so this is 0 to 2
in vec4 gVertWorldLocation[];
in vec4 gNormal[];
in vec4 gUVx2[];

// out to Fragment Shader
out vec4 fVertexColour;
out vec4 fVertWorldLocation;
out vec4 fNormal;
out vec4 fUVx2;
// If we are drawing the debug face normals, then the 4th values here is 1.0f
// Otherwise it's 0.0f and we use the colours as specified elsewhere.
out vec4 fDebugColourOverride;

// The 1st one is for the height map
uniform bool bGenerateNormals;		// Default is 0 (or FALSE)
uniform bool bDrawDebugNormals;		// Default is 0 (or FALSE)
uniform float debugNormalLength;

//uniform mat4 matModel;
uniform mat4 matView;
uniform mat4 matProjection;
uniform mat4 matModelInverseTranspose;


in vec3 gTangentViewPos[];
in vec3 gTangentFragPos[];
in vec3 gTangentLightPos[];
out vec3 fTangentViewPos;
out vec3 fTangentFragPos;
out vec3 fTangentLightPos;

in mat3 gTBN[];
out mat3 fTBN;

void main ()
{
	// This 1st one has the wrong winding, pointing the normals in the wrong way...
	// vec3 theNormal = cross(  vec3(gVertWorldLocation[2]) - vec3(gVertWorldLocation[0]),
	//							vec3(gVertWorldLocation[1]) - vec3(gVertWorldLocation[0]) );
	vec3 triangleNormal = cross(	vec3(gVertWorldLocation[1].xyz) - vec3(gVertWorldLocation[0].xyz),
									vec3(gVertWorldLocation[2].xyz) - vec3(gVertWorldLocation[0].xyz) );
	// AND I forgot to normalize the normal.  Doh!!
	triangleNormal = normalize(triangleNormal);

	//mat4 MVP = matProjection * matView * matModel;
	//gl_Position = MVP * vertPosition;
	gl_Position = gl_in[0].gl_Position;
	fVertexColour = gVertexColour[0];
	fVertWorldLocation = gVertWorldLocation[0];
	if ( bGenerateNormals )
	{
		fNormal = vec4(triangleNormal, 1.0f);
		//fNormal = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else 
	{
		fNormal = gNormal[0];	// vec4(theNormal, 1.0f);
	}
	// vec4(1.0f, 0.0f, 0.0f, 1.0f);	//gNormal[0];
	fUVx2 = gUVx2[0];
	fDebugColourOverride = vec4(1.0f, 1.0f, 1.0f, 0.0f);
	EmitVertex();

	gl_Position = gl_in[1].gl_Position;
	fVertexColour = gVertexColour[1];
	fVertWorldLocation = gVertWorldLocation[1];
	if ( bGenerateNormals )
	{
		fNormal = vec4(triangleNormal, 1.0f); 
//		fNormal = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else
	{
		fNormal = gNormal[1];	// vec4(theNormal, 1.0f); 
	}
	//vec4(0.0f, 1.0f, 0.0f, 1.0f) ;	// gNormal[1];
	fUVx2 = gUVx2[1];
	fDebugColourOverride = vec4(1.0f, 1.0f, 1.0f, 0.0f);
	EmitVertex();
	
	gl_Position = gl_in[2].gl_Position;
	fVertexColour = gVertexColour[2];
	fVertWorldLocation = gVertWorldLocation[2];
	if ( bGenerateNormals )
	{
		fNormal = vec4(triangleNormal, 1.0f); 
//		fNormal = vec4(0.0f, 1.0f, 0.0f, 1.0f);
	}
	else
	{
		fNormal = gNormal[2];	// vec4(theNormal, 1.0f); 
	}
	// vec4(0.0f, 0.0f, 1.0f, 1.0f) ;	// gNormal[2];
	fUVx2 = gUVx2[2];
	fDebugColourOverride = vec4(1.0f, 1.0f, 1.0f, 0.0f);
	EmitVertex();

	EndPrimitive();

	if ( bDrawDebugNormals )
	{
		// We have to do all this stuff we would do in the vertex 
		//	shader because these are BRAND NEW vertices.
		// This triangle is in WORLD space, not model space.
		// So we are already calculating it as it it's in the correct location
		// So this "world" matrix for this generated normal is an identiy matrix
		mat4 matModelGeneratedNormal = mat4(1.0f);
		mat4 matMVPGenNormal = matProjection * matView * matModelGeneratedNormal;

		vec4 triCentre = ( gVertWorldLocation[0] + 
						   gVertWorldLocation[1] +
					       gVertWorldLocation[2] ) / 3.0f;
		
		// Vertex 1 is a little to the -ve of the centre:
		//vec4 vert0Position = triCentre + vec4( -1.0f, 0.0f, 0.0f, 0.0f );
		vec4 vert0Position = triCentre + vec4( -0.05f, 0.0f, 0.0f, 0.0f );
		//vec4 vert1Position = triCentre + vec4( +1.0f, 0.0f, 0.0f, 0.0f );
		vec4 vert1Position = triCentre + vec4( +0.05f, 0.0f, 0.0f, 0.0f );
//		const float NORMAL_LENGTH = 10.0f;
		vec4 vert2Position = vec4( triCentre.xyz + (triangleNormal.xyz * debugNormalLength), 1.0f );
		
// I need to calcualte and output all of these:		
// out gl_Position
//out vec4 fVertexColour;		
//out vec4 fVertWorldLocation;
//out vec4 fNormal;
//out vec4 fUVx2;		

		vec4 yellowRGBA = vec4( 0.0f, 1.0f, 0.0f, 1.0f );


		// Vertex 0:
		gl_Position = matMVPGenNormal * vert0Position; 	
		fVertexColour.rgba = yellowRGBA;
		// This is already in "world space" so I don't need to multiply it by a matrix
		// But I could: fVertWorldLocation = matModelGeneratedNormal * vert1Position;	
		fVertWorldLocation = vert0Position;	
		fNormal.xyz = cross( vec3(vert1Position - vert0Position),
                             vec3(vert2Position - vert0Position) );
		fNormal.w = 1.0f;	// If in doubt, set 4th value to 1.0f
		fNormal = normalize(fNormal);
		// Pick some location on the texture (how about the lower left hand part)
		fUVx2 = vec4( 0.0f, 0.0f, 0.0f, 0.0f );
		fDebugColourOverride = yellowRGBA;
		EmitVertex();	

		// Vertex 1:
		gl_Position = matMVPGenNormal * vert1Position; 	
		fVertexColour.rgba = vec4( 1.0f, 1.0f, 0.0f, 1.0f );	// Yellow
		// This is already in "world space" so I don't need to multiply it by a matrix
		// But I could: fVertWorldLocation = matModelGeneratedNormal * vert1Position;	
		fVertWorldLocation = vert1Position;	
		fNormal.xyz = cross( vec3(vert0Position - vert1Position),
                             vec3(vert2Position - vert1Position) );
		fNormal.w = 1.0f;	// If in doubt, set 4th value to 1.0f
		fNormal = normalize(fNormal);
		// Pick some location on the texture (how about the lower left hand part)
		fUVx2 = vec4( 0.0f, 0.0f, 0.0f, 0.0f );
		fDebugColourOverride = yellowRGBA;
		EmitVertex();		

		// Vertex 1:
		gl_Position = matMVPGenNormal * vert2Position; 	
		fVertexColour.rgba = vec4( 1.0f, 1.0f, 0.0f, 1.0f );	// Yellow
		// This is already in "world space" so I don't need to multiply it by a matrix
		// But I could: fVertWorldLocation = matModelGeneratedNormal * vert1Position;	
		fVertWorldLocation = vert2Position;	
		fNormal.xyz = cross( vec3(vert0Position - vert2Position),
                             vec3(vert1Position - vert2Position) );
		fNormal.w = 1.0f;	// If in doubt, set 4th value to 1.0f
		fNormal = normalize(fNormal);
		// Pick some location on the texture (how about the lower left hand part)
		fUVx2 = vec4( 0.0f, 0.0f, 0.0f, 0.0f );
		fDebugColourOverride = yellowRGBA;
		EmitVertex();		
	
		EndPrimitive();
	} //if ( bDrawNormals )

	fTangentFragPos = gTangentFragPos[0];
	fTangentViewPos = gTangentViewPos[0];

	fTBN = gTBN[0];
}



//	gl_Position = gl_in[0].gl_Position;
//	EmitVertex();	
//	gl_Position = gl_in[1].gl_Position;
//	EmitVertex();
//	gl_Position = vertCentre;//	gl_in[2].gl_Position;
//	EmitVertex();


//	gl_Position = MVP * (gVertWorldLocation[0] + vec3(-1.0f, 1.0f, 0.0f);
//	fVertexColour = gVertexColour[0];
//	fVertWorldLocation = gVertWorldLocation[0];
//	fNormal = theNormal; //gNormal[0];			// vec4(1.0f, 0.0f, 0.0f, 1.0f) ;	// gNormal[0];
//	fUVx2 = gUVx2[0];
//	EmitVertex();	
	
//	gl_Position = MVP * (gVertWorldLocation[0] + vec3(1.0f, 1.0f, 0.0f);
//	fVertexColour = gVertexColour[0];
//	fVertWorldLocation = gVertWorldLocation[0];
//	fNormal = theNormal; //gNormal[0];			// vec4(1.0f, 0.0f, 0.0f, 1.0f) ;	// gNormal[0];
//	fUVx2 = gUVx2[0];
//	EmitVertex();	
	
//	gl_Position = MVP * (gVertWorldLocation[0] + vec3(1.0f, -1.0f, 0.0f);
//	fVertexColour = gVertexColour[0];
//	fVertWorldLocation = gVertWorldLocation[0];
//	fNormal = theNormal; //gNormal[0];			// vec4(1.0f, 0.0f, 0.0f, 1.0f) ;	// gNormal[0];
//	fUVx2 = gUVx2[0];
//	EmitVertex();	
