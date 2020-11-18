using System;

namespace Ay
{
    public class Vector2
    {
        public float x;
        public float y;

        public Vector2(float x, float y)
        {
            this.x = x;
            this.y = y;
        }

        public Vector2(float x) : this(x, x)
        {
        }

        public Vector2() : this(0f)
        {
        }

        #region Class Methods
        public static Vector2 operator+(Vector2 other) => other;
        public static Vector2 operator-(Vector2 other) => new Vector2(-other.x, -other.y);
        public static Vector2 operator+(Vector2 self, Vector2 other) => new Vector2(self.x + other.x, self.y + other.y);
        public static Vector2 operator-(Vector2 self, Vector2 other) => new Vector2(self.x - other.x, self.y - other.y);

        public static Vector2 Cross(Vector2 self, Vector2 other) 
        {
            return new Vector2(
                self.y * other.x - self.x * other.y,
                self.x * other.y - self.y * other.x
            );
        }

        public static float Dot(Vector2 self, Vector2 other) 
        {
            return self.x * other.x + self.y * other.y;
        }

        public static float Length(Vector2 self)
        {
            return (float)Math.Sqrt(self.x * self.x + self.y * self.y);
        }

        public static Vector2 Normalize(Vector2 self)
        {
            float len = Length(self);
            if (len > 0f) {
                return new Vector2(
                    self.x / len,
                    self.y / len                
                );
            }
            return self;
        }
        #endregion

        #region Instance Methods
        public Vector2 Cross(Vector2 other) => Cross(this, other);
        public float Dot(Vector2 other) => Dot(this, other);
        public float Length() => Length(this);
        public Vector2 Normalize() => Normalize(this);
        #endregion
    }

    public class Vector3
    {
        public float x;
        public float y;
        public float z;

        public Vector3(float x, float y, float z)
        {
            this.x = x;
            this.y = y;
            this.z = z;
        }

        public Vector3(float x) : this(x, x, x)
        {
        }

        public Vector3() : this(0f)
        {
        }

        #region Class Methods
        public static Vector3 operator+(Vector3 other) => other;
        public static Vector3 operator-(Vector3 other) => new Vector3(-other.x, -other.y, -other.z);
        public static Vector3 operator+(Vector3 self, Vector3 other) => new Vector3(self.x + other.x, self.y + other.y, self.z + other.z);
        public static Vector3 operator-(Vector3 self, Vector3 other) => new Vector3(self.x - other.x, self.y - other.y, self.z - other.z);

        public static Vector3 Cross(Vector3 self, Vector3 other) 
        {
            return new Vector3(
                self.y * other.z - self.z * other.y,
                self.z * other.x - self.x * other.z,
                self.x * other.y - self.y * other.x
            );
        }

        public static float Dot(Vector3 self, Vector3 other) 
        {
            return self.x * other.x + self.y * other.y + self.z * other.z;
        }

        public static float Length(Vector3 self)
        {
            return (float)Math.Sqrt(self.x * self.x + self.y * self.y + self.z * self.z);
        }

        public static Vector3 Normalize(Vector3 self)
        {
            float len = Length(self);
            if (len > 0f) {
                return new Vector3(
                    self.x / len,
                    self.y / len,
                    self.z / len               
                );
            }
            return self;
        }
        #endregion

        #region Instance Methods
        public Vector3 Cross(Vector3 other) => Cross(this, other);
        public float Dot(Vector3 other) => Dot(this, other);
        public float Length() => Length(this);
        public Vector3 Normalize() => Normalize(this);
        #endregion
    }

    public class Vector4
    {
        public float x;
        public float y;
        public float z;
        public float w;

        public Vector4(float x, float y, float z, float w)
        {
            this.x = x;
            this.y = y;
            this.z = z;
            this.w = w;
        }

        public Vector4(float x) : this(x, x, x, x)
        {
        }

        public Vector4() : this(0f)
        {
        }

        public Vector3 ToVec3() => new Vector3(x / w, y / w, z / w);
    }
}