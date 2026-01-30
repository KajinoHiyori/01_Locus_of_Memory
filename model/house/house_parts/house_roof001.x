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
 16.31181;15.88234;-29.66494;,
 75.44225;134.14334;49.17561;,
 -62.52866;134.14334;49.17561;,
 -62.52866;15.88234;-29.66494;,
 -78.65159;150.41570;54.44368;,
 -78.86361;0.08375;-47.90745;,
 5.66360;0.08375;-47.90745;,
 78.86360;150.41570;54.44368;,
 78.86360;150.41570;54.44368;,
 5.66360;0.08375;-47.90745;,
 5.66360;4.49244;-54.44368;,
 78.65177;158.71624;49.53025;,
 78.65177;158.71624;49.53025;,
 5.66360;4.49244;-54.44368;,
 -78.59308;4.49244;-54.44358;,
 -78.59308;158.71624;49.53025;;
 
 6;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;15,14,5,4;,
 4;5,14,13,6;;
 
 MeshMaterialList {
  9;
  6;
  1,
  4,
  0,
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
  0.000000;-0.554700;0.832051;,
  0.000000;-0.562781;0.826606;,
  0.924926;-0.250807;-0.285671;,
  -0.000001;0.559003;-0.829166;,
  -0.999790;0.012011;-0.016634;,
  -0.000000;-0.829039;-0.559191;;
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
  -0.406650;-3.376710;,
  1.504730;-5.741930;,
  -0.639740;-5.741930;,
  -1.632060;-3.376710;,
  0.309805;-1.171286;,
  -0.496008;0.499069;,
  0.168101;0.499069;,
  1.547361;-1.171286;,
  2.411021;-2.508314;,
  -0.014954;0.498325;,
  -0.097221;0.410151;,
  2.345886;-2.674325;,
  1.507093;-1.263514;,
  0.116747;0.450084;,
  -0.545235;0.450084;,
  0.271661;-1.263514;;
 }
}
