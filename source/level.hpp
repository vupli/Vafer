#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <SFML/Graphics.hpp>
#include "TinyXML/tinyxml.h"

using namespace std;
using namespace sf;

struct Object
{
  int GetPropertyInt(string name);
  float GetPropertyFloat(string name);
  string GetPropertyString(string name);

  string name;
  string type;
  Rect<float> rect;
  map<string, string> properties;

  Sprite sprite;
};

struct Layer
{
  int opacity;
  vector<Sprite> tiles;
};

class Level
{
public:
  bool LoadFromFile(string filename);
  Object GetObject(string name);
  vector<Object> GetObjects(string name);
  vector<Object> GetAllObjects();
  void Draw(RenderWindow &window);
  Vector2i GetTileSize();

private:
  int width, height, tileWidth, tileHeight;
  int firstTileID;
  Rect<float> drawingBounds;
  Texture tilesetImage;
  vector<Object> objects;
  vector<Layer> layers;
};

///////////////////////////////////////


int Object::GetPropertyInt(string name)
{
  return atoi(properties[name].c_str());
}

float Object::GetPropertyFloat(string name)
{
  return strtof(properties[name].c_str(), NULL);
}

string Object::GetPropertyString(string name)
{
  return properties[name];
}

bool Level::LoadFromFile(string filename)
{
  TiXmlDocument levelFile(filename.c_str());

  // Загружаем XML-карту
  if(!levelFile.LoadFile())
    {
      cout << "Loading level \"" << filename << "\" failed." << endl;
      return false;
    }

  // Работаем с контейнером map
  TiXmlElement *map;
  map = levelFile.FirstChildElement("map");

  // Пример карты: <map version="1.0" orientation="orthogonal"
  // width="10" height="10" tilewidth="34" tileheight="34">
  width = atoi(map->Attribute("width"));
  height = atoi(map->Attribute("height"));
  tileWidth = atoi(map->Attribute("tilewidth"));
  tileHeight = atoi(map->Attribute("tileheight"));

  // Берем описание тайлсета и идентификатор первого тайла
  TiXmlElement *tilesetElement;
  tilesetElement = map->FirstChildElement("tileset");
  firstTileID = atoi(tilesetElement->Attribute("firstgid"));

  // source - путь до картинки в контейнере image
  TiXmlElement *image;
  image = tilesetElement->FirstChildElement("image");
  string imagepath = image->Attribute("source");

  // Пытаемся загрузить тайлсет
  Image img;

  if(!img.loadFromFile(imagepath))
    {
      cout << "Failed to load tile sheet." << endl;
      return false;
    }


  img.createMaskFromColor(Color(255, 255, 255));
  tilesetImage.loadFromImage(img);
  tilesetImage.setSmooth(false);

  // Получаем количество столбцов и строк тайлсета
  unsigned int columns = tilesetImage.getSize().x / static_cast<unsigned int>(tileWidth);
  unsigned int rows = tilesetImage.getSize().y / static_cast<unsigned int>(tileHeight);

  // Вектор из прямоугольников изображений (TextureRect)
  vector<Rect<int> > subRects;

  for(unsigned int y = 0; y < rows; y++)
    for(unsigned int x = 0; x < columns; x++)
      {
        Rect<int> rect;

        rect.top = static_cast<int>( y * static_cast<unsigned int>(tileHeight) );
        rect.height = tileHeight;
        rect.left = static_cast<int>( x * static_cast<unsigned int>(tileWidth) );
        rect.width = tileWidth;

        subRects.push_back(rect);
      }

  // Работа со слоями
  TiXmlElement *layerElement;
  layerElement = map->FirstChildElement("layer");
  while(layerElement)
    {
      Layer layer;

      // Если присутствует opacity, то задаем прозрачность слоя, иначе он полностью непрозрачен
      if (layerElement->Attribute("opacity") != NULL)
        {
          float opacity = strtof(layerElement->Attribute("opacity"), NULL);
          layer.opacity = static_cast<int>(255 * opacity);
        }
      else
        {
          layer.opacity = 255;
        }

      // Контейнер <data>
      TiXmlElement *layerDataElement;
      layerDataElement = layerElement->FirstChildElement("data");

      if(layerDataElement == NULL)
        {
          cout << "Bad map. No layer information found." << endl;
        }

      // Контейнер <tile> - описание тайлов каждого слоя
      TiXmlElement *tileElement;
      tileElement = layerDataElement->FirstChildElement("tile");

      if(tileElement == NULL)
        {
          cout << "Bad map. No tile information found." << endl;
          return false;
        }

      int x = 0;
      int y = 0;

      while(tileElement)
        {
          int tileGID = atoi(tileElement->Attribute("gid"));
          int subRectToUse = tileGID - firstTileID;

          // Устанавливаем TextureRect каждого тайла
          if (subRectToUse >= 0)
            {
              Sprite sprite;
              sprite.setTexture(tilesetImage);
              sprite.setTextureRect(subRects[static_cast<unsigned int>(subRectToUse)]);
              sprite.setPosition(x * tileWidth, y * tileHeight);
              sprite.setColor(Color(255, 255, 255, static_cast<unsigned char>(layer.opacity) ));

              layer.tiles.push_back(sprite);
            }

          tileElement = tileElement->NextSiblingElement("tile");

          x++;
          if (x >= width)
            {
              x = 0;
              y++;
              if(y >= height)
                y = 0;
            }
        }

      layers.push_back(layer);

      layerElement = layerElement->NextSiblingElement("layer");
    }

  // Работа с объектами
  TiXmlElement *objectGroupElement;

  // Если есть слои объектов
  if (map->FirstChildElement("objectgroup") != NULL)
    {
      objectGroupElement = map->FirstChildElement("objectgroup");
      while (objectGroupElement)
        {
          // Контейнер <object>
          TiXmlElement *objectElement;
          objectElement = objectGroupElement->FirstChildElement("object");

          while(objectElement)
            {
              // Получаем все данные - тип, имя, позиция, etc
              string objectType;
              if (objectElement->Attribute("type") != NULL)
                {
                  objectType = objectElement->Attribute("type");
                }
              string objectName;
              if (objectElement->Attribute("name") != NULL)
                {
                  objectName = objectElement->Attribute("name");
                }
              int x = atoi(objectElement->Attribute("x"));
              int y = atoi(objectElement->Attribute("y"));

              int width, height;

              Sprite sprite;
              sprite.setTexture(tilesetImage);
              sprite.setTextureRect(Rect<int>(0,0,0,0));
              sprite.setPosition(x, y);

              if (objectElement->Attribute("width") != NULL)
                {
                  width = atoi(objectElement->Attribute("width"));
                  height = atoi(objectElement->Attribute("height"));
                }
              else
                {
                  width = subRects[static_cast<unsigned int>( atoi(objectElement->Attribute("gid")) - firstTileID )].width;
                  height = subRects[static_cast<unsigned int>( atoi(objectElement->Attribute("gid")) - firstTileID )].height;
                  sprite.setTextureRect(subRects[static_cast<unsigned int>( atoi(objectElement->Attribute("gid")) - firstTileID )]);
                }

              // Экземпляр объекта
              Object object;
              object.name = objectName;
              object.type = objectType;
              object.sprite = sprite;

              Rect <float> objectRect;
              objectRect.top = y;
              objectRect.left = x;
              objectRect.height = height;
              objectRect.width = width;
              object.rect = objectRect;

              // "Переменные" объекта
              TiXmlElement *properties;
              properties = objectElement->FirstChildElement("properties");
              if (properties != NULL)
                {
                  TiXmlElement *prop;
                  prop = properties->FirstChildElement("property");
                  if (prop != NULL)
                    {
                      while(prop)
                        {
                          string propertyName = prop->Attribute("name");
                          string propertyValue = prop->Attribute("value");

                          object.properties[propertyName] = propertyValue;

                          prop = prop->NextSiblingElement("property");
                        }
                    }
                }


              objects.push_back(object);

              objectElement = objectElement->NextSiblingElement("object");
            }
          objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
        }
    }
  else
    {
      cout << "No object layers found..." << endl;
    }

  return true;
}

Object Level::GetObject(string name)
{
  // Только первый объект с заданным именем
  for (unsigned int i = 0; i < objects.size(); i++) {
    if (objects[i].name == name)
      return objects[i];
  }
  return Object();
}

vector<Object> Level::GetObjects(string name)
{
  // Все объекты с заданным именем
  vector<Object> vec;
  for(unsigned int i = 0; i < objects.size(); i++)
    if(objects[i].name == name)
      vec.push_back(objects[i]);

  return vec;
}


vector<Object> Level::GetAllObjects()
{
  return objects;
};


Vector2i Level::GetTileSize()
{
  return Vector2i(tileWidth, tileHeight);
}

void Level::Draw(RenderWindow &window)
{
  // Рисуем все тайлы (объекты не рисуем)
  for(unsigned int layer = 0; layer < layers.size(); layer++)
    for(unsigned int tile = 0; tile < layers[layer].tiles.size(); tile++)
      window.draw(layers[layer].tiles[tile]);
}

#endif
