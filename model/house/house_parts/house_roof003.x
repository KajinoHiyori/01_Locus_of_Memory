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
 21.43311;15.28658;-111.13103;,
 21.43311;15.28658;46.54996;,
 -37.69734;133.54758;125.39050;,
 -37.69734;133.54758;-111.13103;,
 -40.90388;140.45720;-127.67202;,
 -37.74694;157.31482;-127.67202;,
 40.90388;6.85409;-127.67202;,
 30.36668;1.48517;-127.67202;,
 -37.74694;157.31482;127.67202;,
 40.90388;6.85409;32.85653;,
 -37.74694;157.31482;127.67202;,
 -40.90388;140.45720;127.67202;,
 30.36668;1.48517;32.85653;,
 40.90388;6.85409;32.85653;,
 -40.90388;140.45720;127.67202;,
 30.36668;1.48517;32.85653;;
 
 6;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;5,8,9,6;,
 4;10,11,12,13;,
 4;14,4,7,15;,
 4;7,6,9,15;;
 
 MeshMaterialList {
  9;
  6;
  1,
  4,
  4,
  0,
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
  -0.894427;-0.447213;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.886223;0.463258;0.000000;,
  0.465359;-0.304789;0.830990;,
  -0.889810;-0.456331;0.000000;,
  0.453987;-0.891008;-0.000000;;
  6;
  4;0,0,0,0;,
  4;1,1,1,1;,
  4;2,2,2,2;,
  4;3,3,3,3;,
  4;4,4,4,4;,
  4;5,5,5,5;;
 }
 MeshTextureCoords {
  16;
  -0.553170;-3.376710;,
  1.431470;-3.376710;,
  1.504730;-5.741930;,
  -1.472230;-5.741930;,
  -0.824457;-1.060636;,
  -0.799654;-1.247942;,
  -0.181715;0.423843;,
  -0.264503;0.483498;,
  1.206518;-1.247942;,
  1.079517;0.423843;,
  1.520234;-2.646297;,
  1.471166;-2.309144;,
  1.385533;0.470297;,
  1.549311;0.362918;,
  1.181715;-1.060636;,
  0.996728;0.483498;;
 }
}
