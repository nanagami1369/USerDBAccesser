export function LevelToPrintString(level: string) {
  switch (level) {
    case 'ADMIN':
      return '管理者'
    case 'PREM':
      return 'プレミア'
    case 'GEN':
      return '一般'
    case 'TRY':
      return 'お試し'
    default:
      throw new Error('存在しない権限が設定されてます')
  }
}

export function AvailToPrintString(avail: string | boolean | number) {
  if (typeof avail == 'boolean') {
    if (avail) {
      return '有効'
    }
    return '無効'
  }
  if (typeof avail === 'string') {
    if (avail === 'true') {
      return '有効'
    }
    if (avail === 'false') {
      return '無効'
    }
    if (avail === '0') {
      return '無効'
    }
    if (avail === '1') {
      return '有効'
    }
  }
  if (typeof avail === 'number') {
    if (avail === 0) {
      return '無効'
    }
    return '有効'
  }
  throw new Error('存在しない権限が設定されてます')
}
