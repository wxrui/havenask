load('//aios/storage/indexlib:defs.bzl', 'indexlib_cc_library')
package(default_visibility=['//aios/storage/indexlib:__subpackages__'])
indexlib_cc_library(
    name='OnDiskPackIndexIterator',
    srcs=[],
    deps=[
        '//aios/storage/indexlib/index/inverted_index:OnDiskIndexIteratorCreator',
        '//aios/storage/indexlib/index/inverted_index/format:PostingDecoderImpl',
        '//aios/storage/indexlib/index/inverted_index/format/dictionary:DictionaryCreator'
    ]
)
indexlib_cc_library(
    name='PackIndexMerger',
    deps=[
        ':OnDiskPackIndexIterator',
        '//aios/storage/indexlib/index:IIndexFactory',
        '//aios/storage/indexlib/index/inverted_index:InvertedIndexMerger'
    ]
)
