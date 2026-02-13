<information.txtについて>
このファイルには、モーションデータテキストのパスと階層構造モデルテキストのパスを記述します。

・モーションデータテキスト
 - モーションビューワで作成したmotion.txtと同じフォーマットで使えます。
 
・階層構造モデルテキスト
 - モーションビューワで作成したmotion.txtのオフセット部分だけを使います。

それぞれモデルビューワなどと同じように

MOTIONSCRIPT = data/～
PARENTSCRIPT = data/～

のような形で記述します。

<Magicobjectinfo.txtについて>
前提として、ランダムに配置するオブジェクトに関しては複数のテキストファイル(model.txtと同じフォーマット)を用意する形になります。
仕組みとしては、用意された複数のテキストファイルに番号を振り、選ばれた番号のファイルを読み込んで設置します。
このファイルには、その用意したファイルたちのパスを記述します。

MAGICOBJECTSCRIPT = data/～

<model.txtについて>
このファイルにはマップを形作るオブジェクトの配置情報を記述します。
読み込めるのは以下の二つです。

モデル情報				(NUM_MODEL = | MODEL_FILENAME = | MODELSET ～ END_MODELSET)
 - POS, ROT, PARENTTYPE(あれば)
 
魔法ギミック情報		(MAGICEVENTSET ～ END_MAGICEVENTSET)
 - POS, RADIUS, EVENT(ギミックの番号)
 
 <Magicobject.txtについて>
 このファイルにはマップにランダムに配置する魔法関連のオブジェクトや、落ちてる魔法の配置情報を記述します。
 読み込めるのは以下の二つです。
 
 モデル情報				(MODELSET ～ END_MODELSET) ※モデルの番号についてはmodel.txtと共用になります
 - POS, ROT
 
 落ちてる魔法の情報		(DROPMAGICSET ～ END_DROPMAGICSET)
  - TYPE(コマンドタイプ), POS, RADIUS
  
  
