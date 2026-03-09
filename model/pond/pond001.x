xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 16;
 -114.80490;-69.72031;-277.16392;,
 -212.13196;-69.72031;-212.13211;,
 -212.13199;-69.72031;212.13211;,
 -114.80496;-69.72031;277.16390;,
 -277.16385;-69.72031;-114.80508;,
 -277.16387;-69.72031;114.80507;,
 114.80502;-69.72031;-277.16387;,
 0.00000;-69.72031;-300.00000;,
 0.00004;-69.72031;300.00000;,
 114.80504;-69.72031;277.16387;,
 212.13203;-69.72031;-212.13203;,
 212.13203;-69.72031;212.13203;,
 277.16387;-69.72031;-114.80502;,
 277.16387;-69.72031;114.80507;,
 -300.00000;-69.72031;0.00000;,
 300.00000;-69.72031;0.00002;;
 
 8;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;6,7,8,9;,
 4;10,6,9,11;,
 4;12,10,11,13;,
 4;0,3,8,7;,
 3;4,14,5;,
 3;13,15,12;;
 
 MeshMaterialList {
  1;
  8;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Desktop\\freeze.jpg";
   }
  }
 }
 MeshNormals {
  1;
  0.000000;1.000000;0.000000;;
  8;
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  3;0,0,0;,
  3;0,0,0;;
 }
 MeshTextureCoords {
  16;
  0.308725;0.959569;,
  0.146536;0.851753;,
  0.146488;0.148294;,
  0.308663;0.040456;,
  0.038158;0.690385;,
  0.038133;0.309676;,
  0.691337;0.959544;,
  0.500033;0.997420;,
  0.499967;0.002580;,
  0.691276;0.040431;,
  0.853512;0.851706;,
  0.853465;0.148247;,
  0.961867;0.690324;,
  0.961842;0.309615;,
  0.000092;0.500033;,
  0.999908;0.499967;;
 }
}
