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
 12;
 138.00872;15.97805;28.63346;,
 -137.93316;15.97805;28.63346;,
 -137.93316;134.23899;-50.20698;,
 138.00872;134.23899;-50.20698;,
 -154.04817;150.51133;-53.96831;,
 154.04817;150.51133;-53.96831;,
 154.04817;0.17949;47.43209;,
 -154.04817;0.17949;47.43209;,
 154.04817;158.78169;-49.88561;,
 154.04817;4.58818;53.96830;,
 -154.04817;158.78169;-49.88561;,
 -154.04817;4.58818;53.96830;;
 
 6;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;5,8,9,6;,
 4;8,10,11,9;,
 4;10,4,7,11;,
 4;7,6,9,11;;
 
 MeshMaterialList {
  9;
  6;
  1,
  4,
  4,
  4,
  4,
  4;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Desktop\\Locus-of-Memory\\01_Locus_of_Memory\\3D表\現技術\\モデルビューワ Ver.3.0\\data\\TEXTURE\\wall001.jpg";
   }
  }
  Material {
   0.700000;0.681100;0.557200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.728000;0.540000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Pictures\\flooring000.jpg";
   }
  }
  Material {
   0.800000;0.734400;0.668000;1.000000;;
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
    "C:\\Users\\student\\Desktop\\Locus-of-Memory\\01_Locus_of_Memory\\3D表\現技術\\モデルビューワ Ver.3.0\\data\\TEXTURE\\rengawall000.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Desktop\\Locus-of-Memory\\01_Locus_of_Memory\\3D表\現技術\\モデルビューワ Ver.3.0\\data\\TEXTURE\\Door002.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Desktop\\Locus-of-Memory\\01_Locus_of_Memory\\3D表\現技術\\モデルビューワ Ver.3.0\\data\\TEXTURE\\stone000.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Desktop\\Locus-of-Memory\\01_Locus_of_Memory\\3D表\現技術\\モデルビューワ Ver.3.0\\data\\TEXTURE\\renga000.jpg";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "C:\\Users\\student\\Desktop\\Locus-of-Memory\\01_Locus_of_Memory\\3D表\現技術\\モデルビューワ Ver.3.0\\data\\TEXTURE\\renga000.jpg";
   }
  }
 }
 MeshNormals {
  6;
  0.000000;-0.554699;-0.832051;,
  0.000000;-0.559194;-0.829037;,
  1.000000;0.000000;-0.000000;,
  0.000000;0.558635;0.829414;,
  -1.000000;0.000000;0.000000;,
  0.000000;-0.829040;0.559190;;
  6;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;;
 }
 MeshTextureCoords {
  12;
  4.641520;-3.376710;,
  0.352580;-3.376710;,
  -0.639740;-5.741930;,
  3.649200;-5.741930;,
  -1.134332;-1.172348;,
  1.286302;-1.172348;,
  2.082978;0.498006;,
  -0.337655;0.498006;,
  1.318378;-1.264241;,
  2.134332;0.449020;,
  -1.102255;-1.264241;,
  -0.286302;0.449020;;
 }
}
