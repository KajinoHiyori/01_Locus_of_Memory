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
 14;
 -0.00023;0.09178;-8.78442;,
 -0.00023;-54.32579;-8.78442;,
 -2.69153;-54.32579;8.78442;,
 -2.69153;0.09178;8.78442;,
 2.69152;-54.32579;8.78442;,
 2.69152;0.09178;8.78442;,
 -0.00023;-54.32579;-8.78442;,
 -0.00023;0.09178;-8.78442;,
 -0.00023;0.09178;2.92818;,
 -0.00023;0.09178;2.92818;,
 -0.00023;0.09178;2.92818;,
 -0.00023;-54.32579;2.92818;,
 -0.00023;-54.32579;2.92818;,
 -0.00023;-54.32579;2.92818;;
 
 9;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 3;8,0,3;,
 3;9,3,5;,
 3;10,5,7;,
 3;11,2,1;,
 3;12,4,2;,
 3;13,6,4;;
 
 MeshMaterialList {
  3;
  9;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.571200;0.282400;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Desktop\\Locus-of-Memory\\01_Locus_of_Memory\\3D表\現技術\\モデルビューワ Ver.3.0\\data\\TEXTURE\\wood003.jpg";
   }
  }
  Material {
   0.408000;0.216800;0.184800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Desktop\\Locus-of-Memory\\01_Locus_of_Memory\\3D表\現技術\\モデルビューワ Ver.3.0\\data\\TEXTURE\\wood000.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Desktop\\Locus-of-Memory\\01_Locus_of_Memory\\3D表\現技術\\モデルビューワ Ver.3.0\\data\\TEXTURE\\rope.png";
   }
  }
 }
 MeshNormals {
  5;
  0.000000;1.000000;0.000000;,
  -0.988470;0.000000;-0.151420;,
  0.000000;-0.000000;1.000000;,
  0.000000;-1.000000;-0.000000;,
  0.988466;0.000000;-0.151444;;
  9;
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;4,4,4,4;,
  3;0,0,0;,
  3;0,0,0;,
  3;0,0,0;,
  3;3,3,3;,
  3;3,3,3;,
  3;3,3,3;;
 }
 MeshTextureCoords {
  14;
  0.000000;0.000000;,
  0.000000;1.000000;,
  0.333330;1.000000;,
  0.333330;0.000000;,
  0.666670;1.000000;,
  0.666670;0.000000;,
  1.000000;1.000000;,
  1.000000;0.000000;,
  0.166670;0.000000;,
  0.500000;0.000000;,
  0.833330;0.000000;,
  0.166670;1.000000;,
  0.500000;1.000000;,
  0.833330;1.000000;;
 }
}
