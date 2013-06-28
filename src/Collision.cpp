/* 
 * File:   collision.cpp
 * Author: Nick (original version), ahnonay (SFML2 compatibility)
 */
#include <SFML/Graphics.hpp>
#include <map>
#include "Collision.h"

namespace Collision
{
	class BitmaskManager
	{
	public:
		~BitmaskManager() {
			std::map<const sf::Texture*, sf::Uint8*>::const_iterator end = Bitmasks.end();
			for (std::map<const sf::Texture*, sf::Uint8*>::const_iterator iter = Bitmasks.begin(); iter!=end; iter++)
				delete [] iter->second;
		}

		sf::Uint8 GetPixel (const sf::Uint8* mask, const sf::Texture* tex, unsigned int x, unsigned int y) {
			if (x>tex->getSize().x||y>tex->getSize().y)
				return 0;

			return mask[x+y*tex->getSize().x];
		}

		sf::Uint8* GetMask (const sf::Texture* tex) {
			sf::Uint8* mask;
			std::map<const sf::Texture*, sf::Uint8*>::iterator pair = Bitmasks.find(tex);
			if (pair==Bitmasks.end())
			{
				sf::Image img = tex->copyToImage();
				mask = CreateMask (tex, img);
			}
			else
				mask = pair->second;

			return mask;
		}

		sf::Uint8* CreateMask (const sf::Texture* tex, const sf::Image& img) {
			sf::Uint8* mask = new sf::Uint8[tex->getSize().y*tex->getSize().x];

			for (unsigned int y = 0; y<tex->getSize().y; y++)
			{
				for (unsigned int x = 0; x<tex->getSize().x; x++)
					mask[x+y*tex->getSize().x] = img.getPixel(x,y).a;
			}

			Bitmasks.insert(std::pair<const sf::Texture*, sf::Uint8*>(tex,mask));

			return mask;
		}
	private:
		std::map<const sf::Texture*, sf::Uint8*> Bitmasks;
	};
	
	BitmaskManager Bitmasks;
 
	bool PixelPerfectTest(const sf::Sprite& Object1, const sf::Sprite& Object2, sf::Uint8 AlphaLimit) {
		sf::FloatRect Intersection; 
		if (Object1.getGlobalBounds().intersects(Object2.getGlobalBounds(), Intersection)) {
			sf::IntRect O1SubRect = Object1.getTextureRect();
			sf::IntRect O2SubRect = Object2.getTextureRect();

			sf::Uint8* mask1 = Bitmasks.GetMask(Object1.getTexture());
			sf::Uint8* mask2 = Bitmasks.GetMask(Object2.getTexture());

			// Loop through our pixels
			for (int i = Intersection.left; i < Intersection.left+Intersection.width; i++) {
				for (int j = Intersection.top; j < Intersection.top+Intersection.height; j++) {
 
					sf::Vector2f o1v = Object1.getInverseTransform().transformPoint(i, j);
					sf::Vector2f o2v = Object2.getInverseTransform().transformPoint(i, j);
 
					// Make sure pixels fall within the sprite's subrect
					if (o1v.x > 0 && o1v.y > 0 && o2v.x > 0 && o2v.y > 0 &&
						o1v.x < O1SubRect.width && o1v.y < O1SubRect.height &&
						o2v.x < O2SubRect.width && o2v.y < O2SubRect.height) {

							if (Bitmasks.GetPixel(mask1, Object1.getTexture(), (int)(o1v.x)+O1SubRect.left, (int)(o1v.y)+O1SubRect.top) > AlphaLimit &&
								Bitmasks.GetPixel(mask2, Object2.getTexture(), (int)(o2v.x)+O2SubRect.left, (int)(o2v.y)+O2SubRect.top) > AlphaLimit)
								return true;

					}
				}
			}
		}
		return false;
	}

	bool CreateTextureAndBitmask(sf::Texture &LoadInto, const std::string& Filename)
	{
		sf::Image img;
		if (!img.loadFromFile(Filename))
			return false;
		if (!LoadInto.loadFromImage(img))
			return false;

		Bitmasks.CreateMask(&LoadInto, img);
		return true;
	}

	sf::Vector2f GetSpriteCenter (const sf::Sprite& Object)
	{
		sf::FloatRect AABB = Object.getGlobalBounds();
		return sf::Vector2f (AABB.left+AABB.width/2.f, AABB.top+AABB.height/2.f);
	}

	sf::Vector2f GetSpriteSize (const sf::Sprite& Object)
	{
		sf::IntRect OriginalSize = Object.getTextureRect();
		sf::Vector2f Scale = Object.getScale();
		return sf::Vector2f (OriginalSize.width*Scale.x, OriginalSize.height*Scale.y);
	}

	bool CircleTest(const sf::Sprite& Object1, const sf::Sprite& Object2) {
		sf::Vector2f Obj1Size = GetSpriteSize(Object1);
		sf::Vector2f Obj2Size = GetSpriteSize(Object2);
		float Radius1 = (Obj1Size.x + Obj1Size.y) / 4;
		float Radius2 = (Obj2Size.x + Obj2Size.y) / 4;

		sf::Vector2f Distance = GetSpriteCenter(Object1)-GetSpriteCenter(Object2);

		return (Distance.x * Distance.x + Distance.y * Distance.y <= (Radius1 + Radius2) * (Radius1 + Radius2));
	}

	bool BoundingBoxTest(const sf::Sprite& Object1, const sf::Sprite& Object2) {
		sf::Vector2f A, B, C, BL, TR;
		sf::Vector2f HalfSize1 = GetSpriteSize(Object1)/2.f;
		sf::Vector2f HalfSize2 = GetSpriteSize(Object2)/2.f;

		//Get the Angle we're working on
		const double RadToDeg = 3.14159265358979323846/180.0;
		float Angle = Object1.getRotation() - Object2.getRotation();
		float CosA = cos(Angle * RadToDeg);
		float SinA = sin(Angle * RadToDeg);
 
		float t, x, a, dx, ext1, ext2;
 
		//Normalise the Center of Object2 so its axis aligned an represented in
		//relation to Object 1
		C = GetSpriteCenter(Object2) - GetSpriteCenter(Object1);
 
		float O2Rot = Object2.getRotation() * RadToDeg;
		float temp = C.x;
		C.x = temp * cos(O2Rot) + C.y * sin(O2Rot);
		C.y = -temp * sin(O2Rot) + C.y * cos(O2Rot);
 
		//Get the Corners
		BL = TR = C;
		BL -= HalfSize2;
		TR += HalfSize2;
 
		//Calculate the vertices of the rotate Rect
		A.x = -HalfSize1.y*SinA;
		B.x = A.x;
		t = HalfSize1.x*CosA;
		A.x += t;
		B.x -= t;
 
		A.y = HalfSize1.y*CosA;
		B.y = A.y;
		t = HalfSize1.x*SinA;
		A.y += t;
		B.y -= t;
 
		t = SinA * CosA;
 
		// verify that A is vertical min/max, B is horizontal min/max
		if (t < 0) {
			t = A.x;
			A.x = B.x;
			B.x = t;
			t = A.y;
			A.y = B.y;
			B.y = t;
		}
 
		// verify that B is horizontal minimum (leftest-vertex)
		if (SinA < 0) {
			B.x = -B.x;
			B.y = -B.y;
		}
 
		// if rr2(ma) isn't in the horizontal range of
		// colliding with rr1(r), collision is impossible
		if (B.x > TR.x || B.x > -BL.x) return false;
 
		// if rr1(r) is axis-aligned, vertical min/max are easy to get
		if (t == 0) {
			ext1 = A.y;
			ext2 = -ext1;
		}// else, find vertical min/max in the range [BL.x, TR.x]
		else {
			x = BL.x - A.x;
			a = TR.x - A.x;
			ext1 = A.y;
			// if the first vertical min/max isn't in (BL.x, TR.x), then
			// find the vertical min/max on BL.x or on TR.x
			if (a * x > 0) {
				dx = A.x;
				if (x < 0) {
					dx -= B.x;
					ext1 -= B.y;
					x = a;
				} else {
					dx += B.x;
					ext1 += B.y;
				}
				ext1 *= x;
				ext1 /= dx;
				ext1 += A.y;
			}
 
			x = BL.x + A.x;
			a = TR.x + A.x;
			ext2 = -A.y;
			// if the second vertical min/max isn't in (BL.x, TR.x), then
			// find the local vertical min/max on BL.x or on TR.x
			if (a * x > 0) {
				dx = -A.x;
				if (x < 0) {
					dx -= B.x;
					ext2 -= B.y;
					x = a;
				} else {
					dx += B.x;
					ext2 += B.y;
				}
				ext2 *= x;
				ext2 /= dx;
				ext2 -= A.y;
			}
		}
 
		// check whether rr2(ma) is in the vertical range of colliding with rr1(r)
		// (for the horizontal range of rr2)
		return !((ext1 < BL.y && ext2 < BL.y) ||
				(ext1 > TR.y && ext2 > TR.y));
	}
}
