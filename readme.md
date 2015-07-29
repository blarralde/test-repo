update!

```
class GithubTranslator
  attr_reader :project

  def initialize project, navigator
    @navigator = navigator
    @project = project
  end

  def readme_to_story! readme
    content = readme.force_encoding('UTF-8')
    markdown = Redcarpet::Markdown.new(Redcarpet::Render::ProjectRenderer, Redcarpet::PROJECT_MARKDOWN_FILTERS)
    html = markdown.render(content)

    doc = Nokogiri::HTML::DocumentFragment.parse html

    project.widgets.where(type: 'ImageWidget').destroy_all

    doc.css('img').reverse.each do |img|
      if next_sibling = img.next_sibling and next_sibling.name == 'br'
        next_sibling.remove
      end
      if img.parent and img.parent.name.in? %w(p ul ol li span)
        img.parent.after img
      end
    end
  end
```

# how it came about

check this video:

http://vimeo.com/22439234

Pictures:

<!--![the caption](https://hackster.imgix.net/uploads/image/file/41981/Ubidots_test_setup.jpg?w=1280&h=960&fit=clip&s=dc0053641464672cfcee0745d325474d)-->

![the caption](https://hackster.imgix.net/uploads/image/file/41972/Ubidots_SPARK_current_draw_mA_scale.PNG?w=1280&h=960&fit=clip&s=9686f57a91b3da3355cf07dcd49c200e)
![the caption](https://hackster.imgix.net/uploads/image/file/41977/Ubidots_Wireshark_IPv6_transaction.PNG?w=1280&h=960&fit=clip&s=f46238c5fc08901b1405c1fe3e415a7a)

![the caption](./files/story/teensy31.jpg)
