module HW1.T3
  ( Tree (..)
  , tsize
  , tdepth
  , tmember
  , tinsert
  , tFromList
  ) where

-- note: do not use error in your implementation - you will lose points :)
type Meta = (Int, Int) -- sz, depth

data Tree a = Leaf | Branch Meta (Tree a) a (Tree a)
  deriving (Show)

tsize :: Tree a -> Int
tsize Leaf                      = 0
tsize (Branch (sz, _) _ _ _) = sz

tdepth :: Tree a -> Int
tdepth Leaf                       = 0
tdepth (Branch (_ , dp) _ _ _) = dp

tvalue :: Tree a -> a
tvalue Leaf = error "Cannot get value from Leaf"
tvalue (Branch _ _ v _) = v

mkBranch :: Tree a -> a -> Tree a -> Tree a
mkBranch Leaf v Leaf = Branch (1, 1) Leaf v Leaf
mkBranch l@(Branch (sz, d) _ _ _) v Leaf = Branch (sz + 1, d + 1) l v Leaf
mkBranch Leaf v r@(Branch (sz, d) _ _ _) = Branch (sz + 1, d + 1) Leaf v r
mkBranch l@(Branch (sz1, d1) _ _ _) v r@(Branch (sz2, d2) _ _ _) = Branch (sz1 + sz2 + 1, d1 `max` d2 + 1) l v r

mkNode :: a -> Tree a
mkNode v = mkBranch Leaf v Leaf

tmember :: Ord a => a -> Tree a -> Bool
tmember _ Leaf = False
tmember k (Branch _ l v r) | v == k = True
                           | k < v = tmember k l
                           | otherwise = tmember k r

tDiff :: Tree a -> Int
tDiff Leaf = 0
tDiff (Branch _ l _ r) = tdepth l - tdepth r

rotateLeft :: Tree a -> Tree a
rotateLeft (Branch _ l v (Branch _ lr vr rr)) = mkBranch (mkBranch l v lr) vr rr
rotateLeft _  = error "Incorrect state. Cannot rotate left"

rotateRight :: Tree a -> Tree a
rotateRight (Branch _ (Branch _ ll vl rl) v r) = mkBranch ll vl (mkBranch rl v r)
rotateRight _  = error "Incorrect state. Cannot rotate right"

tBalance :: Ord a => a -> Tree a -> Tree a
tBalance _ Leaf = Leaf
tBalance v t@(Branch _ l vt r) | (abs diff) <= 1 = t
                               | diff > 1 && v < tvalue l = rotateRight t
                               | diff > 1 && v >= tvalue l = rotateRight (mkBranch (rotateLeft l) vt r)
                               | v > tvalue r = rotateLeft t
                               | otherwise = rotateLeft (mkBranch l vt (rotateRight r))
                               where diff = tDiff t

tinsert :: Ord a => a -> Tree a -> Tree a
tinsert v Leaf = mkNode v
tinsert v t@(Branch _ l vt r) | v == vt = tBalance v t
                              | v < vt = tBalance v (mkBranch (tinsert v l) vt r)
                              | otherwise = tBalance v (mkBranch l vt (tinsert v r))

tFromList :: Ord a => [a] -> Tree a
tFromList xs = foldr tinsert Leaf xs
