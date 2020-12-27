(defn parse-mesh [filename]
  (let [[n-of-vertices-str & tail] (->> (slurp filename) clojure.string/split-lines)
        n-of-vertices (read-string n-of-vertices-str)
        [vertices newtail] (split-at n-of-vertices tail)
        [_ & triangles] newtail
        strs-to-nums (fn [strs] (map (fn [s] (read-string (str "(" s ")"))) strs))
        vertices (strs-to-nums vertices)
        triangles (strs-to-nums triangles)]
    (map (fn [triangle]
           (let [[vi1, vi2, vi3] triangle]
             [(nth vertices vi1)
              (nth vertices vi2)
              (nth vertices vi3)
              ]))
         triangles)))

; (defn to-file [path form]
;   (with-open [w (java.io.FileWriter. (clojure.java.io/file path))]
;     (print-dup form w)))

; (defn from-file [path]
;   (with-open [r (java.io.PushbackReader. (java.io.FileReader. (clojure.java.io/file path)))]
;      (read r)))

; Pseudo:
;
; doos(driehoeken):
;
;     randen = bereken randen (driehoeken)
;     linkerdriehoeken = bereken linkerdriehoeken (driehoeken)
;     rechterdriehoeken = bereken rechterdriehoeken (driehoeken)
;
;     als # linkerdriehoeken > N:
;         linkerdoos = doos(linkerdriehoeken)
;     anders
;         linkerdoos = linkerdriehoeken
;
;     als # rechterdriehoeken > N:
;         rechterdoos = doos(rechterdriehoeken)
;     anders:
;         rechterdoos = rechterdriehoeken
;
;     return (randen, linkerdoos, rechterdoos)
;
;
; rootdoos = doos(alle driehoeken)

(def DEBUG true)

; (defn find-minmax-vertices [triangles]
;  (->> triangles
;       (apply concat) ; flatten triangles to one big vertex list
;       (apply map vector) ; pivot vertexs to xs, ys, zs
;       (map (fn [coords]
;              [(apply max coords) (apply min coords)]))
;       (apply map vector))) ;pivot minmax vals to back to vertexes
(defn minmax [[[sminx sminy sminz] [smaxx smaxy smaxz]] [nxx nxy nxz]]
  (list
    (list (min sminx nxx)
          (min sminy nxy)
          (min sminz nxz))
    (list (max smaxx nxx)
          (max smaxy nxy)
          (max smaxz nxz)
          )))

(defn find-minmax-vertices [triangles]
  (let [[head & tail] (apply concat triangles)]
    (reduce minmax [head head] tail)))

(defn divide-box [boxvertex1 boxvertex2]
  (if DEBUG (print "dividing " boxvertex1 " and " boxvertex2))

  (let [[x1 y1 z1] boxvertex1
        [x2 y2 z2] boxvertex2
        x (Math/abs (- x1 x2))
        y (Math/abs (- y1 y2))
        z (Math/abs (- z1 z2))
        [longest & others] (->> [["x" x] ["y" y] ["z" z]] (sort-by second) reverse)
        halved [(first longest) (/ (second longest) 2)]
        [x y z] (->> (conj others halved) sort (map second))
        emptyleftbox [boxvertex1 [(- x1 x)
                                  (- y1 y)
                                  (- z1 z)]]
        emptyrightbox [[(+ x2 x)
                        (+ y2 y)
                        (+ z2 z)] boxvertex2]]

        [emptyleftbox emptyrightbox]
        ))

(defn find-completly-inside [triangles box]
  (if DEBUG (print "find inside: " (type box)))

  (let [[[x1 y1 z1] [x2 y2 z2]] box]
    (filter (fn [triangle]
              (let [[xs ys zs] (apply map vector triangle)]
                (every? identity [(>= x1 (apply max xs))
                                  (<= x2 (apply min xs))
                                  (>= y1 (apply max ys))
                                  (<= y2 (apply min ys))
                                  (>= z1 (apply max zs))
                                  (<= z2 (apply min zs))]
                        )))
            triangles)))

; (defn makebox [triangles]
;   (if DEBUG (print "triangles: " (count triangles)))

;   (let [[boxvertex1 boxvertex2] (find-minmax-vertices triangles)
;         [emptyleftbox emptyrightbox] (divide-box boxvertex1 boxvertex2)
;         left-triangles (find-completly-inside triangles emptyleftbox)
;         right-triangles (find-completly-inside triangles emptyrightbox)

;         leftbox (cond (> (count left-triangles) N) (makebox left-triangles)
;                       :else left-triangles)
;         rightbox (cond (> (count right-triangles) N) (makebox right-triangles)
;                       :else right-triangles)]
;     {:boxvertices [boxvertex1 boxvertex2]
;      :leftbox leftbox
;      :rightbox rightbox}))

(defn makeboxV2 [N triangles]
  (if DEBUG (print "triangles: " (count triangles)))

  (let [[boxvertex1 boxvertex2] (find-minmax-vertices triangles)
        [emptyleftbox emptyrightbox] (divide-box boxvertex1 boxvertex2)
        left-triangles (find-completly-inside triangles emptyleftbox)
        right-triangles (find-completly-inside triangles emptyrightbox)
        leftover-triangles (clojure.set/difference (set triangles) (set (concat left-triangles right-triangles)))
        [firsthalf secondhalf] (split-at (/ (count leftover-triangles) 2) leftover-triangles)
        left-triangles (concat left-triangles firsthalf)
        right-triangles (concat right-triangles secondhalf)

        leftbox (cond (> (count left-triangles) N) (makeboxV2 N left-triangles)
                      :else left-triangles)
        rightbox (cond (> (count right-triangles) N) (makeboxV2 N right-triangles)
                      :else right-triangles)]
    {:boxvertices [boxvertex1 boxvertex2]
     :leftbox leftbox
     :rightbox rightbox}))

(defn triangle-writer! [writer triangles]
    (doseq [t triangles]
      (.write writer (str "triangle " (clojure.string/join " " (flatten t)) "\n")))
    (.write writer (str (if (empty? triangles) "emptybox" "box") "\n")))

(defn box-writer! [writer box]
  (let [lb (box :leftbox)
        rb (box :rightbox)
        lb-isbox (= (type lb) clojure.lang.PersistentArrayMap)
        rb-isbox (= (type rb) clojure.lang.PersistentArrayMap)]
  (if lb-isbox
    (box-writer! writer lb)
    (triangle-writer! writer lb))
  (if rb-isbox
    (box-writer! writer rb )
    (triangle-writer! writer rb))
  (.write writer "box\n")))

(defn write-optimized-mesh-file! [from to N]
  (with-open [writer (clojure.java.io/writer to)]
      (->> (parse-mesh from) (makeboxV2 N) (box-writer! writer))))
