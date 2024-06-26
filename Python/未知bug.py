class Solution:
    def mySqrt(self, x: int) -> int:

        l,r=0,x
        ans=-1

        while l<=r:
            mid=int(l+(r-l)//2)
            if mid*mid <=x:
                ans=mid
                l=mid+1
            else:
                r=mid-1
        return ans

sl=Solution()

print(sl.mySqrt(1))


